#include "SceneForwardRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "Scene.h"

#include "CameraComponent.h"

#include "StaticMeshComponent.h"
#include "StaticMeshAsset.h"

#include "SkeletalMeshComponent.h"
#include "SkeletalMeshAsset.h"

#include "ModelMaterialAsset.h"
#include "IBLMaterialAsset.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructuredBuffer.h"

using namespace std;

SceneForwardRenderer::SceneForwardRenderer(
	ID3D11DeviceContext* const* deviceContextAddress,
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress,
    Scene* const* sceneAddress
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager, cameraComponentAddress, sceneAddress)
{
}

void SceneForwardRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
	static GraphicsPSOObject* staticMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::STATIC_MESH_FORWARD);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    const IBLMaterialAsset* sceneMaterialAsset = (*m_sceneAddressCached)->GetIBLMaterialAsset();

    if (cameraComponent != nullptr && staticMeshComponent != nullptr && sceneMaterialAsset != nullptr)
    {
        if (const StaticMeshAsset* staticMeshAsset = staticMeshComponent->GetStaticMetalAsset())
        {
            if (MeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(GetLODLevel(staticMeshComponent)))
            {
                staticMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);

                ApplyMainFilmWithIDChannel(deviceContext, cameraComponent);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                    staticMeshComponent->GetTransformationBuffer()->GetBuffer()
                };
                deviceContext->VSSetConstantBuffers(0, 2, vsConstantBuffers.data());
                // ===================================================================

                // =============================== DS ===============================
                vector<ID3D11Buffer*> dsConstantBuffers{
                    cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                    staticMeshComponent->GetTransformationBuffer()->GetBuffer()
                };
                deviceContext->DSSetConstantBuffers(0, 2, dsConstantBuffers.data());
                // ===================================================================

                // =============================== PS ===============================
                vector<ID3D11Buffer*> psConstantBuffers{
                    staticMeshComponent->GetComponentBuffer()->GetBuffer(),
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

                m_selectedModelMaterialAssets = staticMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(deviceContext, meshPartsData, bind(&SceneForwardRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }
}

void SceneForwardRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_FORWARD);

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

                ApplyMainFilmWithIDChannel(deviceContext, cameraComponent);

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
                RenderMeshParts(deviceContext, meshPartsData, bind(&SceneForwardRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }

}

void SceneForwardRenderer::Visit(CameraComponent* cameraComponent)
{

}

void SceneForwardRenderer::ApplyMainFilmWithIDChannel(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent)
{
    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = cameraComponent->GetFilm();
    const Texture2DInstance<RTVOption>* const idFilm = cameraComponent->GetIDFilm();

    const Texture2DInstance<DSVOption>* const depthStencilView = cameraComponent->GetDepthStencilViewBuffer();

    vector<ID3D11RenderTargetView*> rtvs{ film->GetRTV(), idFilm->GetRTV() };

    deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), depthStencilView->GetDSV());
    deviceContext->RSSetViewports(1, cameraComponent);

}

void SceneForwardRenderer::PostProcess()
{
}
