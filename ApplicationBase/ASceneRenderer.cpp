#include "ASceneRenderer.h"
#include "ComponentPSOManager.h"

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
    CameraComponent* const* cameraComponentAddress
)
	: m_deviceContextAddress(deviceContextAddress), 
    m_componentPsoManagerCached(componentPsoManager), 
    m_selectedCameraComponentAddressCached(cameraComponentAddress)
{
}

void ASceneRenderer::Visit(Scene* scene)
{   
    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const selectedCameraComponentCached = *m_selectedCameraComponentAddressCached;

    if (selectedCameraComponentCached != nullptr)
    {
        AShader* const sceneVertexShader = m_componentPsoManagerCached->GetComponentPSOVertexShader(ComponentPSOManager::EComponentPSOVertexShader::SCENE);
        AShader* const scenePixelShader = m_componentPsoManagerCached->GetComponentPSOPixelShader(ComponentPSOManager::EComponentPSOPixelShader::FORWARD_SCENE);
        ID3D11SamplerState* const wrapSamplerState = m_componentPsoManagerCached->GetComponentPSOSamplerState(ComponentPSOManager::EComponentPSOSamplerState::WRAP);
        ID3D11DepthStencilState* const lessDepthStencilState = m_componentPsoManagerCached->GetComponentPSODepthStencilState(ComponentPSOManager::EComponentPSODeptshStencilState::DEPTH_COMPARE_LESS);
        ID3D11RasterizerState* const ccwSolidSSRasterizerState = m_componentPsoManagerCached->GetComponentPSORasterizerState(ComponentPSOManager::EComponentPSORasterizerState::CCW_SOLID_SS);
        const vector<ID3D11SamplerState*> samplerStates = { m_componentPsoManagerCached->GetComponentPSOSamplerState(ComponentPSOManager::EComponentPSOSamplerState::WRAP) };
        
        const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = selectedCameraComponentCached->GetFilm();
        const Texture2DInstance<DSVOption>* const depthStencilView = selectedCameraComponentCached->GetDepthStencilViewBuffer();


        vector<ID3D11RenderTargetView*> rtvs{ film->GetRTV() };

        deviceContext->OMSetRenderTargets(1, rtvs.data(), depthStencilView->GetDSV());
        deviceContext->OMSetDepthStencilState(lessDepthStencilState, 0);
    
        deviceContext->RSSetState(ccwSolidSSRasterizerState);
        deviceContext->RSSetViewports(1, selectedCameraComponentCached);

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

        for (size_t idx = 0; idx < meshPartCount; ++idx)
        {
            const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

            sceneVertexShader->SetShader(deviceContext);
            scenePixelShader->SetShader(deviceContext);

            deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
            deviceContext->IASetIndexBuffer(meshPartsData->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, indicesOffsets[idx]);

            vector<ID3D11Buffer*> vsConstantBuffers{ selectedCameraComponentCached->GetViewProjMatrixBuffer()->GetBuffer() };
            vector<ID3D11Buffer*> psConstantBuffers{ iblMaterialAsset->GetIBLToneMappingBuffer()->GetBuffer() };
            vector<ID3D11ShaderResourceView*> psSRVs{ backgroundTextureAsset->GetSRV() };

            deviceContext->VSSetConstantBuffers(0, 1, vsConstantBuffers.data());
            deviceContext->PSSetConstantBuffers(0, 1, psConstantBuffers.data());
            deviceContext->PSSetShaderResources(0, 1, psSRVs.data());
            deviceContext->PSSetSamplers(0, 1, samplerStates.data());
            deviceContext->DrawIndexed(indexCount, NULL, NULL);
        }
    }
}
