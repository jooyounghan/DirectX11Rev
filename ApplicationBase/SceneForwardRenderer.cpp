#include "SceneForwardRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "Scene.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "StaticMeshAsset.h"
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
	ID3D11DeviceContext* m_deviceContext,
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress,
    Scene* const* sceneAddress
)
	: ASceneRenderer(m_deviceContext, componentPsoManager, cameraComponentAddress, sceneAddress)
{
}

void SceneForwardRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
    static GraphicsPSOObject* staticMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::STATIC_MESH_FORWARD);

    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    const IBLMaterialAsset* sceneMaterialAsset = (*m_sceneAddressCached)->GetIBLMaterialAsset();
    if (cameraComponent != nullptr && staticMeshComponent != nullptr && sceneMaterialAsset != nullptr)
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
                vector<ID3D11ShaderResourceView*> psSRVs
                {
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_SPECULAR)->GetSRV(),
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_DIFFUSE)->GetSRV(),
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF)->GetSRV()
                };
                m_deviceContext->PSSetConstantBuffers(0, 2, psConstantBuffers.data());
                m_deviceContext->PSSetShaderResources(0, 3, psSRVs.data());
                // ===================================================================

                m_selectedModelMaterialAssets = staticMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(m_deviceContext, meshPartsData, bind(&SceneForwardRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }
}

void SceneForwardRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
	static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_FORWARD);

    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    const IBLMaterialAsset* sceneMaterialAsset = (*m_sceneAddressCached)->GetIBLMaterialAsset();
    if (cameraComponent != nullptr && skeletalMeshComponent != nullptr && sceneMaterialAsset != nullptr)
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
                vector<ID3D11ShaderResourceView*> psSRVs
                {
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_SPECULAR)->GetSRV(),
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_DIFFUSE)->GetSRV(),
                    sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF)->GetSRV()
                };
                m_deviceContext->PSSetConstantBuffers(0, 2, psConstantBuffers.data());
                m_deviceContext->PSSetShaderResources(0, 3, psSRVs.data());
                // ===================================================================

                m_selectedModelMaterialAssets = skeletalMeshComponent->GetSelectedModelMaterials();
                RenderMeshParts(m_deviceContext, meshPartsData, bind(&SceneForwardRenderer::RenderMeshPartHandler, this, placeholders::_1));
                m_selectedModelMaterialAssets.clear();
            }
        }
    }
}

void SceneForwardRenderer::Visit(CameraComponent* cameraComponent)
{

}

void SceneForwardRenderer::PostProcess()
{
}

void SceneForwardRenderer::RenderMeshPartHandler(const size_t& idx)
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
        m_deviceContext->PSSetShaderResources(3, 7, psSRVsPerMeshPart.data());
        // ===================================================================
    }

}
