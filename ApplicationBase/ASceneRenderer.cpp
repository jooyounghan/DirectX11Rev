#include "ASceneRenderer.h"

#include "AssetManager.h"
#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "Scene.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

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

#include "PerformanceAnalyzer.h"

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
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(0))
            {
                graphicsPSOObject->ApplyPSOObject(deviceContext);
                ASceneRenderer::ApplyRenderTargets(deviceContext, cameraComponent);

                const IBLMaterialAsset* const iblMaterialAsset = scene->GetIBLMaterialAsset();
                const ScratchTextureAsset* const backgroundTextureAsset = iblMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BACKGROUND);

                vector<ID3D11Buffer*> vsConstantBuffers{ cameraComponent->GetViewProjMatrixBuffer()->GetBuffer() };
                vector<ID3D11Buffer*> psConstantBuffers{ iblMaterialAsset->GetIBLToneMappingBuffer()->GetBuffer() };
                vector<ID3D11ShaderResourceView*> psSRVs{ backgroundTextureAsset->GetSRV() };

                deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(vsConstantBuffers.size()), vsConstantBuffers.data());
                deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());
                deviceContext->PSSetShaderResources(0, static_cast<UINT>(psSRVs.size()), psSRVs.data());

                RenderMeshParts(deviceContext, meshPartsData);
            }
        }
    }
}

void ASceneRenderer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
    RenderCollisionComponent(sphereCollisionComponent, StaticMeshAsset::DefaultSphereModelName);
    sphereCollisionComponent->SetRenderable(false);
}

void ASceneRenderer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
    RenderCollisionComponent(orientedBoxCollisionComponent, StaticMeshAsset::DefaultCubeModelName);
    orientedBoxCollisionComponent->SetRenderable(false);
}

void ASceneRenderer::Visit(SpotLightComponent* spotLightComponent)
{
}

void ASceneRenderer::Visit(PointLightComponent* pointLightComponent)
{
}

void ASceneRenderer::ClearRenderTargets()
{
    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    if (CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached)
    {
        deviceContext->ClearRenderTargetView(
            cameraComponent->GetFilm()->GetRTV(), ClearColor
        );
        deviceContext->ClearRenderTargetView(
            cameraComponent->GetIDFilm()->GetRTV(), ClearColor
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

void ASceneRenderer::ApplyRenderTargets(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent) const
{
    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = cameraComponent->GetFilm();
    const Texture2DInstance<DSVOption>* const depthStencilView = cameraComponent->GetDepthStencilViewBuffer();

    vector<ID3D11RenderTargetView*> rtvs{ film->GetRTV() };

    deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), depthStencilView->GetDSV());
    deviceContext->RSSetViewports(1, cameraComponent);
}

void ASceneRenderer::ApplyRenderTargetsWithID(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent) const
{
    const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const film = cameraComponent->GetFilm();
    const Texture2DInstance<RTVOption>* const idFilm = cameraComponent->GetIDFilm();
    const Texture2DInstance<DSVOption>* const depthStencilView = cameraComponent->GetDepthStencilViewBuffer();

    vector<ID3D11RenderTargetView*> rtvs{ film->GetRTV(), idFilm->GetRTV()};

    deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), depthStencilView->GetDSV());
    deviceContext->RSSetViewports(1, cameraComponent);
}

void ASceneRenderer::RenderCollisionComponent(ACollisionComponent* collisionComponent, const string& debugStaticMeshAssetName)
{
    static GraphicsPSOObject* graphicsPSOObject = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::DEBUG_COMPONENT);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;

    if (cameraComponent != nullptr && graphicsPSOObject != nullptr)
    {
        AssetManager* assetManager = AssetManager::GetInstance();
        if (const StaticMeshAsset* staticMeshAsset = assetManager->GetStaticMeshAsset(debugStaticMeshAssetName))
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(0))
            {
                graphicsPSOObject->ApplyPSOObject(deviceContext);
                ASceneRenderer::ApplyRenderTargetsWithID(deviceContext, cameraComponent);

                vector<ID3D11Buffer*> vsConstantBuffers{
                    cameraComponent->GetViewProjMatrixBuffer()->GetBuffer(),
                    collisionComponent->GetTransformationBuffer()->GetBuffer()
                };
                vector<ID3D11Buffer*> psConstantBuffers{ collisionComponent->GetComponentBuffer()->GetBuffer() };

                deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(vsConstantBuffers.size()), vsConstantBuffers.data());
                deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());

                RenderMeshParts(deviceContext, meshPartsData);
            }
        }
    }
}

void ASceneRenderer::RenderMeshParts(
    ID3D11DeviceContext* const deviceContext, 
    const AMeshPartsData* const meshPartsData,
    function<void(const size_t&)> handler
)
{
    const size_t& meshPartCount = meshPartsData->GetPartsCount();
    const vector<UINT>& indicesOffsets = meshPartsData->GetIndexOffsets();

    const vector<ConstantBuffer*> constantVertexBuffers = meshPartsData->GetVertexConstantBuffers();
    vector<ID3D11Buffer*> vertexBuffers = meshPartsData->GetVertexBuffers();    
    const vector<UINT>& strides = meshPartsData->GetStrides();
    const vector<UINT>& verticesOffsets = meshPartsData->GetOffsets();
    const UINT& totalIndicesCount = static_cast<UINT>(meshPartsData->GetIndices().size());

    for (size_t idx = 0; idx < meshPartCount; ++idx)
    {
        const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

        handler(idx);

        deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
        deviceContext->IASetIndexBuffer(meshPartsData->GetIndexBuffer()->GetBuffer(), DXGI_FORMAT_R32_UINT, NULL);
        deviceContext->DrawIndexed(indexCount, indicesOffsets[idx], NULL);
        PerformanceAnalyzer::DrawCount += indexCount;
    }
}
