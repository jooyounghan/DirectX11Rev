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

    if (cameraComponent != nullptr && skeletalMeshComponent != nullptr && sceneMaterialAsset != nullptr)
    {
        if (const SkeletalMeshAsset* skeletalMeshAsset = skeletalMeshComponent->GetSkeletalMetalAsset())
        {
            if (MeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(GetLODLevel(skeletalMeshComponent)))
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

void SceneForwardRenderer::PostProcess()
{
}

void SceneForwardRenderer::RenderMeshPartHandler(const size_t& idx)
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
        deviceContext->PSSetShaderResources(3, 7, psSRVsPerMeshPart.data());
        // ===================================================================
    }

}
