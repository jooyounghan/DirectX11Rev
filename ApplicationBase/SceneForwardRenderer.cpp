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
}

void SceneForwardRenderer::Visit(SkeletalMeshComponent* skeletalMeshComopnent)
{
	static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
		= m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_FORWARD);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    const IBLMaterialAsset* sceneMaterialAsset = (*m_sceneAddressCached)->GetIBLMaterialAsset();

    if (cameraComponent != nullptr && skeletalMeshGraphicsPSOObject != nullptr)
    {
        skeletalMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);
        ApplyCamera();

        const SkeletalMeshAsset* const skeletalMeshAsset = skeletalMeshComopnent->GetSkeletalMetalAsset();
        MeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(GetLODLevel(skeletalMeshComopnent));
        const vector<ModelMaterialAsset*>& selectedModelMaterialAssets = skeletalMeshComopnent->GetSelectedModelMaterials();

        const size_t& meshPartCount = meshPartsData->GetPartsCount();
        const vector<UINT>& indicesOffsets = meshPartsData->GetIndexOffsets();
        const vector<ID3D11Buffer*> vertexBuffers = meshPartsData->GetD3D11VertexBuffers();
        const vector<UINT>& strides = meshPartsData->GetStrides();
        const vector<UINT>& verticesOffsets = meshPartsData->GetOffsets();
        const UINT& totalIndicesCount = static_cast<UINT>(meshPartsData->GetIndices().size());

        for (size_t idx = 0; idx < meshPartCount; ++idx)
        {
            const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

            deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
            deviceContext->IASetIndexBuffer(meshPartsData->GetD3D11IndexBuffer(), DXGI_FORMAT_R32_UINT, NULL);

            vector<ID3D11Buffer*> vsConstantBuffers { 
                cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                skeletalMeshComopnent->GetTransformationBuffer()->GetBuffer()
            };
            vector<ID3D11Buffer*> dsConstantBuffers {
                cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                skeletalMeshComopnent->GetTransformationBuffer()->GetBuffer(),
                selectedModelMaterialAssets[idx]->GetModelTextureSettingBuffer()->GetBuffer()
            };
            vector<ID3D11Buffer*> psConstantBuffers{
                skeletalMeshComopnent->GetComponentBuffer()->GetBuffer(),
                cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                selectedModelMaterialAssets[idx]->GetModelTextureSettingBuffer()->GetBuffer()
            };

            const ModelMaterialAsset* modelMaterialAsset = selectedModelMaterialAssets[idx];

            const BaseTextureAsset* ambientocculusionAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_AMBIENTOCCULUSION);
            const BaseTextureAsset* specularAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_SPECULAR);
            const BaseTextureAsset* diffuseAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_DIFFUSE);
            const BaseTextureAsset* roughnessAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_ROUGHNESS);
            const BaseTextureAsset* metalicAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_METALIC);
            const BaseTextureAsset* normalAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_NORMAL);
            const BaseTextureAsset* emissiveAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_EMISSIVE);
            const BaseTextureAsset* heightAsset = modelMaterialAsset->GetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_HEIGHT);

            vector<ID3D11ShaderResourceView*> vsSRVs{ skeletalMeshComopnent->GetAnimationPlayer()->GetBoneTransformationBuffer()->GetSRV() };
            vector<ID3D11ShaderResourceView*> dsSRVs{ heightAsset ? heightAsset->GetSRV() : nullptr };
            vector<ID3D11ShaderResourceView*> psSRVs
            {
                sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_SPECULAR)->GetSRV(),
                sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_DIFFUSE)->GetSRV(),
                sceneMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF)->GetSRV(),
                ambientocculusionAsset ? ambientocculusionAsset->GetSRV() : nullptr,
                specularAsset ? specularAsset->GetSRV() : nullptr,
                diffuseAsset ? diffuseAsset->GetSRV() : nullptr,
                roughnessAsset ? roughnessAsset->GetSRV() : nullptr,
                metalicAsset ? metalicAsset->GetSRV() : nullptr,
                normalAsset ? normalAsset->GetSRV() : nullptr,
                emissiveAsset ? emissiveAsset->GetSRV() : nullptr
            };


            deviceContext->VSSetConstantBuffers(0, 2, vsConstantBuffers.data());
            deviceContext->VSSetShaderResources(0, 1, vsSRVs.data());

            deviceContext->DSSetConstantBuffers(0, 3, dsConstantBuffers.data());
            deviceContext->DSSetShaderResources(0, 1, dsSRVs.data());

            deviceContext->PSSetConstantBuffers(0, 3, psConstantBuffers.data());
            deviceContext->PSSetShaderResources(0, 10, psSRVs.data());

            deviceContext->DrawIndexed(indexCount, indicesOffsets[idx], NULL);
        }
    }

}

void SceneForwardRenderer::Visit(CameraComponent* cameraComponent)
{

}
