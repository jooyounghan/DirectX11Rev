#include "SceneForwardRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "CameraComponent.h"

#include "StaticMeshComponent.h"
#include "StaticMeshAsset.h"

#include "SkeletalMeshComponent.h"
#include "SkeletalMeshAsset.h"

#include "ModelMaterialAsset.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"

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

    if (cameraComponent != nullptr && skeletalMeshGraphicsPSOObject != nullptr)
    {
        skeletalMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);
        ApplyCamera();

        const SkeletalMeshAsset* const skeletalMeshAsset = skeletalMeshComopnent->GetSkeletalMetalAsset();
        MeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(GetLODLevel(skeletalMeshComopnent));
        const vector<ModelMaterialAsset*>& selectedModelMaterialAssets = skeletalMeshComopnent->GetSelectedModelMaterials();

        const size_t& meshPartCount = meshPartsData->GetPartsCount();
        const vector<UINT>& indicesOffsets = meshPartsData->GetIndexOffsets();
        const vector<ID3D11Buffer*> vertexBuffers = meshPartsData->GetVertexBuffers();
        const vector<UINT>& strides = meshPartsData->GetStrides();
        const vector<UINT>& verticesOffsets = meshPartsData->GetOffsets();
        const UINT& totalIndicesCount = static_cast<UINT>(meshPartsData->GetIndices().size());

        for (size_t idx = 0; idx < meshPartCount; ++idx)
        {
            const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

            deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
            deviceContext->IASetIndexBuffer(meshPartsData->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, indicesOffsets[idx]);

            vector<ID3D11Buffer*> vsConstantBuffers { 
                cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                skeletalMeshComopnent->GetTransformationBuffer()->GetBuffer(),
            };
            vector<ID3D11Buffer*> dsConstantBuffers {
                cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                skeletalMeshComopnent->GetTransformationBuffer()->GetBuffer(),
                nullptr /* TextureSet */
            };
            vector<ID3D11Buffer*> psConstantBuffers{
                skeletalMeshComopnent->GetComponentBuffer()->GetBuffer(),
                cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                nullptr /* Texture Set */
            };

            vector<ID3D11ShaderResourceView*> vsSRVs{ nullptr /* Texture Set */};
            vector<ID3D11ShaderResourceView*> dsSRVs{ nullptr /* Height Map */};
            vector<ID3D11ShaderResourceView*> psSRVs{ nullptr /* Height Map */ };


            //deviceContext->VSSetConstantBuffers(0, 1, vsConstantBuffers.data());
            //deviceContext->PSSetConstantBuffers(0, 1, psConstantBuffers.data());
            //deviceContext->PSSetShaderResources(0, 1, psSRVs.data());

            deviceContext->DrawIndexed(indexCount, NULL, NULL);
        }
    }

}

void SceneForwardRenderer::Visit(CameraComponent* cameraComponent)
{

}
