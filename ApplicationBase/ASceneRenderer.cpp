#include "ASceneRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "Scene.h"
#include "CameraComponent.h"

#include "StaticMeshAsset.h"
#include "IBLMaterialAsset.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

#include "DynamicBuffer.h"

using namespace std;

ASceneRenderer::ASceneRenderer(
	ID3D11DeviceContext* const* deviceContextAddress,
	ComponentPSOManager* componentPsoManager,
    CameraComponent* const* cameraComponentAddress,
    Scene* const* sceneAddress
)
	: m_deviceContextAddress(deviceContextAddress), 
    m_componentPsoManagerCached(componentPsoManager), 
    m_selectedCameraComponentAddressCached(cameraComponentAddress),
    m_sceneAddressCached(sceneAddress)
{
}

void ASceneRenderer::Visit(Scene* scene)
{   
    static GraphicsPSOObject* graphicsPSOObject = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SCENE);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;

    if (cameraComponent != nullptr && graphicsPSOObject != nullptr)
    {
        graphicsPSOObject->ApplyPSOObject(deviceContext);
        ApplyCamera();
    
        const StaticMeshAsset* const staticMeshAsset = scene->GetSceneMeshAsset();
        MeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(NULL);

        const IBLMaterialAsset* const iblMaterialAsset = scene->GetIBLMaterialAsset();
        const ScratchTextureAsset* const backgroundTextureAsset = iblMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BACKGROUND);

        const size_t& meshPartCount = meshPartsData->GetPartsCount();
        const vector<UINT>& indicesOffsets = meshPartsData->GetIndexOffsets();
        const vector<ID3D11Buffer*> vertexBuffers = meshPartsData->GetVertexBuffers();
        const vector<UINT>& strides = meshPartsData->GetStrides();
        const vector<UINT>& verticesOffsets = meshPartsData->GetOffsets();
        const UINT& totalIndicesCount = static_cast<UINT>(meshPartsData->GetIndices().size());

        vector<ID3D11Buffer*> vsConstantBuffers{ cameraComponent->GetViewProjMatrixBuffer()->GetBuffer() };
        vector<ID3D11Buffer*> psConstantBuffers{ iblMaterialAsset->GetIBLToneMappingBuffer()->GetBuffer() };
        vector<ID3D11ShaderResourceView*> psSRVs{ backgroundTextureAsset->GetSRV() };

        deviceContext->VSSetConstantBuffers(0, 1, vsConstantBuffers.data());
        deviceContext->PSSetConstantBuffers(0, 1, psConstantBuffers.data());
        deviceContext->PSSetShaderResources(0, 1, psSRVs.data());

        for (size_t idx = 0; idx < meshPartCount; ++idx)
        {
            const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

            deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
            deviceContext->IASetIndexBuffer(meshPartsData->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, indicesOffsets[idx]);
            deviceContext->DrawIndexed(indexCount, NULL, NULL);
        }
    }
}

void ASceneRenderer::ApplyCamera() const
{
    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;

    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = cameraComponent->GetFilm();
    const Texture2DInstance<DSVOption>* const depthStencilView = cameraComponent->GetDepthStencilViewBuffer();

    vector<ID3D11RenderTargetView*> rtvs{ film->GetRTV() };

    deviceContext->OMSetRenderTargets(1, rtvs.data(), depthStencilView->GetDSV());
    deviceContext->RSSetViewports(1, cameraComponent);
}

uint32_t ASceneRenderer::GetLODLevel(const AComponent* component) const
{
    return 0;
}
