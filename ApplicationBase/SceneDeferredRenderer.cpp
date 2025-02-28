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


#include "ScreenQuad.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructuredBuffer.h"


using namespace std;

SceneDeferredRenderer::SceneDeferredRenderer(
    ID3D11Device* device,
    ID3D11DeviceContext* deviceContext,
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress,
	Scene* const* sceneAddress
)
	: ASceneRenderer(deviceContext, componentPsoManager, cameraComponentAddress, sceneAddress),
	m_positionGBuffer(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16G16B16A16_FLOAT),
    m_specularGBuffer(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM),
    m_diffuseGBuffer(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM),
	m_aoMetallicRoughnessGBuffer(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM),
	m_normalGBuffer(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16G16B16A16_FLOAT),
	m_emissiveGBuffer(GDefaultViewWidth, GDefaultViewHeight, 1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM)
{
    m_positionGBuffer.InitializeByOption(device, deviceContext);
    m_specularGBuffer.InitializeByOption(device, deviceContext);
    m_diffuseGBuffer.InitializeByOption(device, deviceContext);
    m_aoMetallicRoughnessGBuffer.InitializeByOption(device, deviceContext);
    m_normalGBuffer.InitializeByOption(device, deviceContext);
    m_emissiveGBuffer.InitializeByOption(device, deviceContext);

    m_gBufferShaderResourceViews = { m_positionGBuffer.GetSRV(), m_specularGBuffer.GetSRV(), m_diffuseGBuffer.GetSRV(),m_aoMetallicRoughnessGBuffer.GetSRV(), m_normalGBuffer.GetSRV(), m_emissiveGBuffer.GetSRV() };
    m_gBufferRenderTargetViews = { m_positionGBuffer.GetRTV(), m_specularGBuffer.GetRTV(), m_diffuseGBuffer.GetRTV(),m_aoMetallicRoughnessGBuffer.GetRTV(), m_normalGBuffer.GetRTV(), m_emissiveGBuffer.GetRTV() };
}

void SceneDeferredRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
	static GraphicsPSOObject* staticMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::STATIC_MESH_DEFERRED);

    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;

    if (cameraComponent != nullptr && staticMeshComponent != nullptr)
    {
        if (const StaticMeshAsset* staticMeshAsset = staticMeshComponent->GetStaticMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(GetLODLevel(staticMeshAsset->GetLODCount(), staticMeshComponent)))
            {
                staticMeshGraphicsPSOObject->ApplyPSOObject(m_deviceContext);

                ApplyRenderTargetsWithID(m_deviceContext, cameraComponent);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    cameraComponent->GetViewEntityBuffer().GetBuffer(),
                    staticMeshComponent->GetTransformationEntityBuffer().GetBuffer()
                };
                m_deviceContext->VSSetConstantBuffers(0, 2, vsConstantBuffers.data());
                // ===================================================================

                // =============================== DS ===============================
                vector<ID3D11Buffer*> dsConstantBuffers{
                    cameraComponent->GetViewEntityBuffer().GetBuffer(),
                    staticMeshComponent->GetTransformationEntityBuffer().GetBuffer()
                };
                m_deviceContext->DSSetConstantBuffers(0, 2, dsConstantBuffers.data());
                // ===================================================================

                // =============================== PS ===============================
                vector<ID3D11Buffer*> psConstantBuffers{
                    staticMeshComponent->GetComponentEntityBuffer().GetBuffer(),
                    cameraComponent->GetViewEntityBuffer().GetBuffer()
                };
                m_deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());
                // ===================================================================

                m_selectedModelMaterialAssets = staticMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(m_deviceContext, meshPartsData, bind(&SceneDeferredRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }
}

void SceneDeferredRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
    static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_DEFERRED);

    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    if (cameraComponent != nullptr && skeletalMeshComponent != nullptr)
    {
        if (const SkeletalMeshAsset* skeletalMeshAsset = skeletalMeshComponent->GetSkeletalMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(GetLODLevel(skeletalMeshAsset->GetLODCount(), skeletalMeshComponent)))
            {
                skeletalMeshGraphicsPSOObject->ApplyPSOObject(m_deviceContext);

                ApplyRenderTargetsWithID(m_deviceContext, cameraComponent);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    cameraComponent->GetViewEntityBuffer().GetBuffer(),
                    skeletalMeshComponent->GetTransformationEntityBuffer().GetBuffer()
                };
                vector<ID3D11ShaderResourceView*> vsSRVs{ skeletalMeshComponent->GetAnimationPlayer()->GetBoneTransformationBuffer()->GetSRV() };

                m_deviceContext->VSSetConstantBuffers(0, 2, vsConstantBuffers.data());
                m_deviceContext->VSSetShaderResources(0, 1, vsSRVs.data());
                // ===================================================================

                // =============================== DS ===============================
                vector<ID3D11Buffer*> dsConstantBuffers{
                    cameraComponent->GetViewEntityBuffer().GetBuffer(),
                    skeletalMeshComponent->GetTransformationEntityBuffer().GetBuffer()
                };
                m_deviceContext->DSSetConstantBuffers(0, 2, dsConstantBuffers.data());
                // ===================================================================

                // =============================== PS ===============================
                vector<ID3D11Buffer*> psConstantBuffers{
                    skeletalMeshComponent->GetComponentEntityBuffer().GetBuffer(),
                    cameraComponent->GetViewEntityBuffer().GetBuffer()
                };                
                m_deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());
                // ===================================================================

                m_selectedModelMaterialAssets = skeletalMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(m_deviceContext, meshPartsData, bind(&SceneDeferredRenderer::RenderMeshPartHandler, this, placeholders::_1));
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

    m_deviceContext->ClearRenderTargetView(m_positionGBuffer.GetRTV(), ClearColor);
    m_deviceContext->ClearRenderTargetView(m_specularGBuffer.GetRTV(), ClearColor);
    m_deviceContext->ClearRenderTargetView(m_diffuseGBuffer.GetRTV(), ClearColor);
    m_deviceContext->ClearRenderTargetView(m_aoMetallicRoughnessGBuffer.GetRTV(), ClearColor);
    m_deviceContext->ClearRenderTargetView(m_normalGBuffer.GetRTV(), ClearColor);
    m_deviceContext->ClearRenderTargetView(m_emissiveGBuffer.GetRTV(), ClearColor);
}

void SceneDeferredRenderer::PostProcess()
{
	static GraphicsPSOObject* graphicsPSOObject = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::DEFERRED_GBUFFER_RESOLVE);

	ScreenQuad* screenQuad = ScreenQuad::GetInstance();

    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    Scene* const selectedScene = *m_sceneAddressCached;
    LightManager& lightManager = selectedScene->GetLightManager();

    const IBLMaterialAsset* sceneMaterialAsset = (*m_sceneAddressCached)->GetIBLMaterialAsset();
	if (selectedScene != nullptr && cameraComponent != nullptr)
	{
		graphicsPSOObject->ApplyPSOObject(m_deviceContext);
		ASceneRenderer::ApplyRenderTargets(m_deviceContext, cameraComponent);

        const vector<SpotLightComponent*>& spotLights = lightManager.GetSpotLights();
        const vector<PointLightComponent*>& pointLights = lightManager.GetPointLights();

        // =============================== PS ===============================
        vector<ID3D11Buffer*> psConstantBuffers{
            cameraComponent->GetViewEntityBuffer().GetBuffer(),
            lightManager.GetLightManagerEntityBuffer().GetBuffer()
        };

        vector<ID3D11ShaderResourceView*> psSRVs
        {
            sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_SPECULAR)->GetSRV(),
            sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_DIFFUSE)->GetSRV(),
            sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF)->GetSRV(),
            m_positionGBuffer.GetSRV(),
            m_specularGBuffer.GetSRV(),
            m_diffuseGBuffer.GetSRV(),
            m_aoMetallicRoughnessGBuffer.GetSRV(),
            m_normalGBuffer.GetSRV(),
            m_emissiveGBuffer.GetSRV(),
            lightManager.GetSpotLightEntityBuffer().GetSRV(),
            lightManager.GetSpotLightViewEntityBuffer().GetSRV(),
            lightManager.GetSpotLightDepthTestViews().GetSRV()
        };

        m_deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());
        m_deviceContext->PSSetShaderResources(0, static_cast<UINT>(psSRVs.size()), psSRVs.data());
        // ===================================================================

		const vector<ID3D11Buffer*> vertexBuffers = screenQuad->GetVertexBuffers();
		const vector<UINT>& strides = screenQuad->GetStrides();
		const vector<UINT>& verticesOffsets = screenQuad->GetOffsets();

		m_deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
		m_deviceContext->IASetIndexBuffer(screenQuad->GetIndexBuffer()->GetBuffer(), DXGI_FORMAT_R32_UINT, NULL);
		m_deviceContext->DrawIndexed(screenQuad->GetIndexCount(), NULL, NULL);

        vector<ID3D11ShaderResourceView*> psNullSRVs;
        psNullSRVs.resize(psSRVs.size(), nullptr);
        m_deviceContext->PSSetShaderResources(0, static_cast<UINT>(psSRVs.size()), psNullSRVs.data());
	}
}

void SceneDeferredRenderer::ApplyRenderTargetsWithID(ID3D11DeviceContext* const deviceContext, CameraComponent* cameraComponent) const
{
    vector<ID3D11RenderTargetView*> rtvs = m_gBufferRenderTargetViews;
    rtvs.push_back(cameraComponent->GetIDFilm().GetRTV());
    Texture2DInstance<DSVOption>& depthStencilView = cameraComponent->GetDepthStencilView();

    deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), depthStencilView.GetDSV());
    deviceContext->RSSetViewports(1, &cameraComponent->GetViewport());
}

void SceneDeferredRenderer::RenderMeshPartHandler(const size_t& idx)
{
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

        m_deviceContext->DSSetConstantBuffers(2, 1, dsConstantBuffersPerMeshPart.data());
        m_deviceContext->DSSetShaderResources(0, 1, dsSRVsPerMeshPart.data());
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
        m_deviceContext->PSSetConstantBuffers(2, 1, psConstantBuffersPerMeshPart.data());
        m_deviceContext->PSSetShaderResources(0, 7, psSRVsPerMeshPart.data());
        // ===================================================================
    }
}
