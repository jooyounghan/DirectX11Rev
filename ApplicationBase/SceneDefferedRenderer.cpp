#include "SceneDefferedRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "CameraComponent.h"

#include "Scene.h"

#include "StaticMeshComponent.h"
#include "StaticMeshAsset.h"

#include "SkeletalMeshComponent.h"
#include "SkeletalMeshAsset.h"

#include "ModelMaterialAsset.h"
#include "IBLMaterialAsset.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"
#include "DSVOption.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructuredBuffer.h"

using namespace std;

SceneDefferedRenderer::SceneDefferedRenderer(
	ID3D11Device* const* deviceAddress,
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress,
	Scene* const* sceneAddress
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager, cameraComponentAddress, sceneAddress)
{
	m_positionGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultWidth, GDefaultHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
	m_baseColorGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultWidth, GDefaultHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
	m_aoMetallicRoughnessGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultWidth, GDefaultHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
	m_normalGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultWidth, GDefaultHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
	m_emissiveGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultWidth, GDefaultHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);

    m_gBufferShaderResourceViews = { m_positionGBuffer->GetSRV(), m_baseColorGBuffer->GetSRV(), m_aoMetallicRoughnessGBuffer->GetSRV(), m_normalGBuffer->GetSRV(), m_emissiveGBuffer->GetSRV() };
    m_gBufferRenderTargetViews = { m_positionGBuffer->GetRTV(), m_baseColorGBuffer->GetRTV(), m_aoMetallicRoughnessGBuffer->GetRTV(), m_normalGBuffer->GetRTV(), m_emissiveGBuffer->GetRTV() };
}

SceneDefferedRenderer::~SceneDefferedRenderer()
{
	delete m_positionGBuffer;
	delete m_baseColorGBuffer;
	delete m_aoMetallicRoughnessGBuffer;
	delete m_normalGBuffer;
	delete m_emissiveGBuffer;
}

void SceneDefferedRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
}

void SceneDefferedRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
    static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_DEFFERED);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    const IBLMaterialAsset* sceneMaterialAsset = (*m_sceneAddressCached)->GetIBLMaterialAsset();

    if (cameraComponent != nullptr && skeletalMeshGraphicsPSOObject != nullptr && sceneMaterialAsset != nullptr)
    {
        if (const SkeletalMeshAsset* skeletalMeshAsset = skeletalMeshComponent->GetSkeletalMetalAsset())
        {
            if (MeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(GetLODLevel(skeletalMeshComponent)))
            {
                skeletalMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);
                
                ApplyGBuffer(deviceContext, cameraComponent);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                    skeletalMeshComponent->GetTransformationBuffer()->GetBuffer()
                };
                vector<ID3D11ShaderResourceView*> vsSRVs{ skeletalMeshComponent->GetAnimationPlayer()->GetBoneTransformationBuffer()->GetSRV() };

                deviceContext->VSSetConstantBuffers(0, 2, vsConstantBuffers.data());
                deviceContext->VSSetShaderResources(0, 1, vsSRVs.data());
                // ===================================================================

                // =============================== DS ===============================
                vector<ID3D11Buffer*> dsConstantBuffers{
                    cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                    skeletalMeshComponent->GetTransformationBuffer()->GetBuffer()
                };
                deviceContext->DSSetConstantBuffers(0, 2, dsConstantBuffers.data());
                // ===================================================================

                // =============================== PS ===============================
                vector<ID3D11Buffer*> psConstantBuffers{
                    skeletalMeshComponent->GetComponentBuffer()->GetBuffer(),
                    cameraComponent->GetViewProjMatrixBuffer()->GetBuffer()
                };
                vector<ID3D11ShaderResourceView*> psSRVs
                {
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_SPECULAR)->GetSRV(),
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_DIFFUSE)->GetSRV(),
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF)->GetSRV()
                };
                deviceContext->PSSetConstantBuffers(0, 2, psConstantBuffers.data());
                deviceContext->PSSetShaderResources(0, 3, psSRVs.data());
                // ===================================================================

                m_selectedModelMaterialAssets = skeletalMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(deviceContext, meshPartsData, bind(&SceneDefferedRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }
}

void SceneDefferedRenderer::Visit(CameraComponent* cameraComponent)
{
}

void SceneDefferedRenderer::ApplyGBuffer(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent)
{
    static FLOAT clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
    const Texture2DInstance<DSVOption>* const depthStencilView = cameraComponent->GetDepthStencilViewBuffer();

    deviceContext->ClearRenderTargetView(m_positionGBuffer->GetRTV(), clearColor);
    deviceContext->ClearRenderTargetView(m_baseColorGBuffer->GetRTV(), clearColor);
    deviceContext->ClearRenderTargetView(m_aoMetallicRoughnessGBuffer->GetRTV(), clearColor);
    deviceContext->ClearRenderTargetView(m_normalGBuffer->GetRTV(), clearColor);
    deviceContext->ClearRenderTargetView(m_emissiveGBuffer->GetRTV(), clearColor);

    deviceContext->ClearDepthStencilView(
        depthStencilView->GetDSV(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0
    );


    deviceContext->OMSetRenderTargets(static_cast<UINT>(m_gBufferRenderTargetViews.size()), m_gBufferRenderTargetViews.data(), depthStencilView->GetDSV());
    deviceContext->RSSetViewports(1, cameraComponent);
}

void SceneDefferedRenderer::PostProcess()
{

}
