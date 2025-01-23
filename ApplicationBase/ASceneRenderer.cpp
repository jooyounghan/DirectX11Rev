#include "ASceneRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "Scene.h"
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
        if (const StaticMeshAsset* staticMeshAsset = scene->GetSceneMeshAsset())
        {
            if (MeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(0))
            {
                graphicsPSOObject->ApplyPSOObject(deviceContext);
                ApplyMainFilmCamera(deviceContext, cameraComponent);

                const IBLMaterialAsset* const iblMaterialAsset = scene->GetIBLMaterialAsset();
                const ScratchTextureAsset* const backgroundTextureAsset = iblMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BACKGROUND);

                vector<ID3D11Buffer*> vsConstantBuffers{ cameraComponent->GetViewProjMatrixBuffer()->GetBuffer() };
                vector<ID3D11Buffer*> psConstantBuffers{ iblMaterialAsset->GetIBLToneMappingBuffer()->GetBuffer() };
                vector<ID3D11ShaderResourceView*> psSRVs{ backgroundTextureAsset->GetSRV() };

                deviceContext->VSSetConstantBuffers(0, 1, vsConstantBuffers.data());
                deviceContext->PSSetConstantBuffers(0, 1, psConstantBuffers.data());
                deviceContext->PSSetShaderResources(0, 1, psSRVs.data());

                RenderMeshParts(deviceContext, meshPartsData);
            }
        }
    }
}

void ASceneRenderer::ClearRenderTargets()
{
    static FLOAT clearColor[4] = { 0.f, 0.f, 0.f, 0.f };

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    if (CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached)
    {
        deviceContext->ClearRenderTargetView(
            cameraComponent->GetFilm()->GetRTV(), clearColor
        );
        deviceContext->ClearRenderTargetView(
            cameraComponent->GetIDFilm()->GetRTV(), clearColor
        );
        deviceContext->ClearDepthStencilView(
            cameraComponent->GetDepthStencilViewBuffer()->GetDSV(),
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0
        );
    }
}

uint32_t ASceneRenderer::GetLODLevel(const AComponent* component) const
{
    return 0;
}

void ASceneRenderer::ApplyMainFilmCamera(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent) const
{
    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = cameraComponent->GetFilm();
    const Texture2DInstance<DSVOption>* const depthStencilView = cameraComponent->GetDepthStencilViewBuffer();

    vector<ID3D11RenderTargetView*> rtvs{ film->GetRTV() };

    deviceContext->OMSetRenderTargets(1, rtvs.data(), depthStencilView->GetDSV());
    deviceContext->RSSetViewports(1, cameraComponent);
}

void ASceneRenderer::RenderMeshParts(
    ID3D11DeviceContext* const deviceContext, 
    const MeshPartsData* const meshPartsData,
    function<void(const size_t&)> handler
)
{
    const size_t& meshPartCount = meshPartsData->GetPartsCount();
    const vector<UINT>& indicesOffsets = meshPartsData->GetIndexOffsets();
    const vector<ID3D11Buffer*> vertexBuffers = meshPartsData->GetD3D11VertexBuffers();
    const vector<UINT>& strides = meshPartsData->GetStrides();
    const vector<UINT>& verticesOffsets = meshPartsData->GetOffsets();
    const UINT& totalIndicesCount = static_cast<UINT>(meshPartsData->GetIndices().size());

    for (size_t idx = 0; idx < meshPartCount; ++idx)
    {
        const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

        handler(idx);

        deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
        deviceContext->IASetIndexBuffer(meshPartsData->GetD3D11IndexBuffer(), DXGI_FORMAT_R32_UINT, NULL);
        deviceContext->DrawIndexed(indexCount, indicesOffsets[idx], NULL);
    }
}

void ASceneRenderer::RenderMeshPartHandler(const size_t& idx)
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
