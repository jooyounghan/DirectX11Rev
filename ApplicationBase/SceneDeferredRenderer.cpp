#include "SceneDeferredRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "CameraComponent.h"

#include "SpotLightComponent.h"
#include "PointLightComponent.h"

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

#include "ScreenQuad.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructuredBuffer.h"


using namespace std;

SceneDeferredRenderer::SceneDeferredRenderer(
	ID3D11Device* const* deviceAddress,
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress,
	Scene* const* sceneAddress
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager, cameraComponentAddress, sceneAddress)
{
	m_positionGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16G16B16A16_FLOAT, *deviceAddress, *deviceContextAddress);
    m_specularGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
    m_diffuseGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
	m_aoMetallicRoughnessGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
	m_normalGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16G16B16A16_FLOAT, *deviceAddress, *deviceContextAddress);
	m_emissiveGBuffer = new Texture2DInstance<SRVOption, RTVOption>(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, *deviceAddress, *deviceContextAddress);
    m_gBufferShaderResourceViews = { m_positionGBuffer->GetSRV(), m_specularGBuffer->GetSRV(), m_diffuseGBuffer->GetSRV(),m_aoMetallicRoughnessGBuffer->GetSRV(), m_normalGBuffer->GetSRV(), m_emissiveGBuffer->GetSRV() };
    m_gBufferRenderTargetViews = { m_positionGBuffer->GetRTV(), m_specularGBuffer->GetRTV(), m_diffuseGBuffer->GetRTV(),m_aoMetallicRoughnessGBuffer->GetRTV(), m_normalGBuffer->GetRTV(), m_emissiveGBuffer->GetRTV() };
}

SceneDeferredRenderer::~SceneDeferredRenderer()
{
	delete m_positionGBuffer;
    delete m_specularGBuffer;
    delete m_diffuseGBuffer;
	delete m_aoMetallicRoughnessGBuffer;
	delete m_normalGBuffer;
	delete m_emissiveGBuffer;
}

void SceneDeferredRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
	static GraphicsPSOObject* staticMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::STATIC_MESH_DEFERRED);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;

    if (cameraComponent != nullptr && staticMeshComponent != nullptr)
    {
        if (const StaticMeshAsset* staticMeshAsset = staticMeshComponent->GetStaticMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(GetLODLevel(staticMeshAsset->GetLODCount(), staticMeshComponent)))
            {
                staticMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);

                ApplyRenderTargetsWithID(deviceContext, cameraComponent);

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
                deviceContext->PSSetConstantBuffers(0, 2, psConstantBuffers.data());
                // ===================================================================

                m_selectedModelMaterialAssets = staticMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(deviceContext, meshPartsData, bind(&SceneDeferredRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }
}

void SceneDeferredRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
    static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_DEFERRED);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;

    if (cameraComponent != nullptr && skeletalMeshComponent != nullptr)
    {
        if (const SkeletalMeshAsset* skeletalMeshAsset = skeletalMeshComponent->GetSkeletalMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(GetLODLevel(skeletalMeshAsset->GetLODCount(), skeletalMeshComponent)))
            {
                skeletalMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);

                ApplyRenderTargetsWithID(deviceContext, cameraComponent);

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
                deviceContext->PSSetConstantBuffers(0, 2, psConstantBuffers.data());
                // ===================================================================

                m_selectedModelMaterialAssets = skeletalMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(deviceContext, meshPartsData, bind(&SceneDeferredRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }
}

void SceneDeferredRenderer::Visit(CameraComponent* cameraComponent)
{
}

void SceneDeferredRenderer::ClearRenderTargets()
{
    ASceneRenderer::ClearRenderTargets();

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    deviceContext->ClearRenderTargetView(m_positionGBuffer->GetRTV(), ClearColor);
    deviceContext->ClearRenderTargetView(m_specularGBuffer->GetRTV(), ClearColor);
    deviceContext->ClearRenderTargetView(m_diffuseGBuffer->GetRTV(), ClearColor);
    deviceContext->ClearRenderTargetView(m_aoMetallicRoughnessGBuffer->GetRTV(), ClearColor);
    deviceContext->ClearRenderTargetView(m_normalGBuffer->GetRTV(), ClearColor);
    deviceContext->ClearRenderTargetView(m_emissiveGBuffer->GetRTV(), ClearColor);
}

void SceneDeferredRenderer::PostProcess()
{
	static GraphicsPSOObject* graphicsPSOObject = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::DEFERRED_GBUFFER_RESOLVE);

	ScreenQuad* screenQuad = ScreenQuad::GetInstance();

	ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    Scene* const scene = *m_sceneAddressCached;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    const IBLMaterialAsset* sceneMaterialAsset = (*m_sceneAddressCached)->GetIBLMaterialAsset();

	if (scene != nullptr && cameraComponent != nullptr)
	{
		graphicsPSOObject->ApplyPSOObject(deviceContext);
		ASceneRenderer::ApplyRenderTargets(deviceContext, cameraComponent);

        const vector<SpotLightComponent*>& spotLights = scene->GetSpotLights();
        const vector<PointLightComponent*>& pointLights = scene->GetPointLights();

        // =============================== PS ===============================
        vector<ID3D11Buffer*> psConstantBuffers{
            cameraComponent->GetViewProjMatrixBuffer()->GetBuffer()
        };

        vector<ID3D11ShaderResourceView*> psSRVs
        {
            sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_SPECULAR)->GetSRV(),
            sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_DIFFUSE)->GetSRV(),
            sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF)->GetSRV(),
            m_positionGBuffer->GetSRV(),
            m_specularGBuffer->GetSRV(),
            m_diffuseGBuffer->GetSRV(),
            m_aoMetallicRoughnessGBuffer->GetSRV(),
            m_normalGBuffer->GetSRV(),
            m_emissiveGBuffer->GetSRV()
        };

        deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());
        deviceContext->PSSetShaderResources(0, static_cast<UINT>(psSRVs.size()), psSRVs.data());
        // ===================================================================

		const vector<ID3D11Buffer*> vertexBuffers = screenQuad->GetVertexBuffers();
		const vector<UINT>& strides = screenQuad->GetStrides();
		const vector<UINT>& verticesOffsets = screenQuad->GetOffsets();

		deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
		deviceContext->IASetIndexBuffer(screenQuad->GetIndexBuffer()->GetBuffer(), DXGI_FORMAT_R32_UINT, NULL);
		deviceContext->DrawIndexed(screenQuad->GetIndexCount(), NULL, NULL);

        vector<ID3D11ShaderResourceView*> psNullSRVs;
        psNullSRVs.resize(psSRVs.size(), nullptr);
        deviceContext->PSSetShaderResources(0, static_cast<UINT>(psSRVs.size()), psNullSRVs.data());
	}
}

void SceneDeferredRenderer::ApplyRenderTargetsWithID(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent) const
{
    vector<ID3D11RenderTargetView*> rtvs = m_gBufferRenderTargetViews;
    rtvs.push_back(cameraComponent->GetIDFilm()->GetRTV());
    const Texture2DInstance<DSVOption>* const depthStencilView = cameraComponent->GetDepthStencilViewBuffer();

    deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), depthStencilView->GetDSV());
    deviceContext->RSSetViewports(1, cameraComponent);
}

void SceneDeferredRenderer::RenderMeshPartHandler(const size_t& idx)
{
    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;

    if (m_selectedModelMaterialAssets.size() > idx)
    {
        const ModelMaterialAsset* modelMaterialAsset = m_selectedModelMaterialAssets[idx];
        const BaseTextureAsset* ambientocculusionAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_AMBIENTOCCULUSION);
        const BaseTextureAsset* specularAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_SPECULAR);
        const BaseTextureAsset* diffuseAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_DIFFUSE);
        const BaseTextureAsset* roughnessAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_ROUGHNESS);
        const BaseTextureAsset* metalicAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_METALIC);
        const BaseTextureAsset* normalAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_NORMAL);
        const BaseTextureAsset* emissiveAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_EMISSIVE);
        const BaseTextureAsset* heightAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_HEIGHT);

        // =============================== DS ===============================
        vector<ID3D11Buffer*> dsConstantBuffersPerMeshPart{
            m_selectedModelMaterialAssets[idx]->GetModelTextureSettingBuffer()->GetBuffer()
        };
        vector<ID3D11ShaderResourceView*> dsSRVsPerMeshPart{ heightAsset ? heightAsset->GetSRV() : nullptr };

        deviceContext->DSSetConstantBuffers(2, 1, dsConstantBuffersPerMeshPart.data());
        deviceContext->DSSetShaderResources(0, 1, dsSRVsPerMeshPart.data());
        // ===================================================================

        // =============================== PS ===============================
        vector<ID3D11Buffer*> psConstantBuffersPerMeshPart{
            m_selectedModelMaterialAssets[idx]->GetModelTextureSettingBuffer()->GetBuffer()
        };
        vector<ID3D11ShaderResourceView*> psSRVsPerMeshPart
        {
            ambientocculusionAsset ? ambientocculusionAsset->GetSRV() : nullptr,
            specularAsset ? specularAsset->GetSRV() : nullptr,
            diffuseAsset ? diffuseAsset->GetSRV() : nullptr,
            roughnessAsset ? roughnessAsset->GetSRV() : nullptr,
            metalicAsset ? metalicAsset->GetSRV() : nullptr,
            normalAsset ? normalAsset->GetSRV() : nullptr,
            emissiveAsset ? emissiveAsset->GetSRV() : nullptr
        };
        deviceContext->PSSetConstantBuffers(2, 1, psConstantBuffersPerMeshPart.data());
        deviceContext->PSSetShaderResources(0, 7, psSRVsPerMeshPart.data());
        // ===================================================================
    }
}
