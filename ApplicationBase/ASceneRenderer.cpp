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
using namespace DirectX;

ASceneRenderer::ASceneRenderer(
	ID3D11DeviceContext* deviceContext,
	ComponentPSOManager* componentPsoManager,
    CameraComponent* const* cameraComponentAddress,
    Scene* const* sceneAddress
)
	: m_deviceContext(deviceContext), 
    m_componentPsoManagerCached(componentPsoManager), 
    m_selectedCameraComponentAddressCached(cameraComponentAddress),
    m_sceneAddressCached(sceneAddress)
{
}

void ASceneRenderer::Visit(Scene* scene)
{   
    static GraphicsPSOObject* graphicsPSOObject = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SCENE);

    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    if (cameraComponent != nullptr && graphicsPSOObject != nullptr)
    {
        if (const StaticMeshAsset* staticMeshAsset = scene->GetSceneMeshAsset())
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(0))
            {
                graphicsPSOObject->ApplyPSOObject(m_deviceContext);
                ASceneRenderer::ApplyRenderTargets(m_deviceContext, cameraComponent);

                const IBLMaterialAsset* const iblMaterialAsset = scene->GetIBLMaterialAsset();
                if (iblMaterialAsset)
                {
                    const ScratchTextureAsset* const backgroundTextureAsset = iblMaterialAsset->GetScratchTextureAsset(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BACKGROUND);
                    
                    vector<ID3D11Buffer*> vsConstantBuffers{ cameraComponent->GetViewEntityBuffer().GetBuffer() };
                    vector<ID3D11Buffer*> psConstantBuffers{ iblMaterialAsset->GetIBLToneMappingBuffer()->GetBuffer() };
                    vector<ID3D11ShaderResourceView*> psSRVs{ backgroundTextureAsset->GetSRV() };

                    m_deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(vsConstantBuffers.size()), vsConstantBuffers.data());
                    m_deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());
                    m_deviceContext->PSSetShaderResources(0, static_cast<UINT>(psSRVs.size()), psSRVs.data());

                    RenderMeshParts(m_deviceContext, meshPartsData);
                }
            }
        }
    }
}

void ASceneRenderer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
    RenderCollisionComponent(sphereCollisionComponent, StaticMeshAsset::DefaultSphereModelName);
}

void ASceneRenderer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
    RenderCollisionComponent(orientedBoxCollisionComponent, StaticMeshAsset::DefaultCubeModelName);
}

void ASceneRenderer::Visit(SpotLightComponent* spotLightComponent)
{
}

void ASceneRenderer::Visit(PointLightComponent* pointLightComponent)
{
}

void ASceneRenderer::ClearRenderTargets()
{
    if (CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached)
    {
        m_deviceContext->ClearRenderTargetView(
            cameraComponent->GetFilm().GetRTV(), ClearColor
        );
        m_deviceContext->ClearRenderTargetView(
            cameraComponent->GetIDFilm().GetRTV(), ClearColor
        );
        m_deviceContext->ClearDepthStencilView(
            cameraComponent->GetDepthStencilView().GetDSV(),
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0
        );
    }
}

uint32_t ASceneRenderer::GetLODLevel(const size_t& maxLODLevel, const AComponent* component) const
{
    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    if (cameraComponent)
    {
        XMVECTOR vDistance = XMVectorSubtract(cameraComponent->GetAbsolutePosition(), component->GetAbsolutePosition());
        const float distance = XMVectorGetX(XMVector3Length(vDistance));
        const float maxDistance = cameraComponent->GetFarZ();
        constexpr int controlOption = 300;

        return static_cast<uint32_t>(maxLODLevel * (1 - pow(1 - (distance / maxDistance), controlOption)));
    }
    return 0;
}

void ASceneRenderer::ApplyRenderTargets(ID3D11DeviceContext* const deviceContext, CameraComponent* const cameraComponent) const
{
    Texture2DInstance<SRVOption, RTVOption, UAVOption>& film = cameraComponent->GetFilm();
    Texture2DInstance<DSVOption>& depthStencilView = cameraComponent->GetDepthStencilView();

    vector<ID3D11RenderTargetView*> rtvs{ film.GetRTV() };

    deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), depthStencilView.GetDSV());
    deviceContext->RSSetViewports(1, &cameraComponent->GetViewport());
}

void ASceneRenderer::ApplyRenderTargetsWithID(ID3D11DeviceContext* const deviceContext, CameraComponent* const cameraComponent) const
{
    Texture2DInstance<SRVOption, RTVOption, UAVOption>& film = cameraComponent->GetFilm();
    Texture2DInstance<RTVOption>& idFilm = cameraComponent->GetIDFilm();
    Texture2DInstance<DSVOption>& depthStencilView = cameraComponent->GetDepthStencilView();

    vector<ID3D11RenderTargetView*> rtvs{ film.GetRTV(), idFilm.GetRTV()};

    deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvs.data(), depthStencilView.GetDSV());
    deviceContext->RSSetViewports(1, &cameraComponent->GetViewport());
}

void ASceneRenderer::RenderCollisionComponent(ACollisionComponent* collisionComponent, const string& debugStaticMeshAssetName)
{
    static GraphicsPSOObject* graphicsPSOObject = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::DEBUG_COMPONENT);

    CameraComponent* const cameraComponent = *m_selectedCameraComponentAddressCached;
    if (cameraComponent != nullptr && graphicsPSOObject != nullptr)
    {
        AssetManager* assetManager = AssetManager::GetInstance();
        if (const StaticMeshAsset* staticMeshAsset = assetManager->GetStaticMeshAsset(debugStaticMeshAssetName))
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(0))
            {
                graphicsPSOObject->ApplyPSOObject(m_deviceContext);
                ASceneRenderer::ApplyRenderTargetsWithID(m_deviceContext, cameraComponent);

                vector<ID3D11Buffer*> vsConstantBuffers{
                    cameraComponent->GetViewEntityBuffer().GetBuffer(),
                    collisionComponent->GetTransformationEntityBuffer().GetBuffer()
                };
                vector<ID3D11Buffer*> psConstantBuffers{ collisionComponent->GetComponentEntityBuffer().GetBuffer() };

                m_deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(vsConstantBuffers.size()), vsConstantBuffers.data());
                m_deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(psConstantBuffers.size()), psConstantBuffers.data());

                RenderMeshParts(m_deviceContext, meshPartsData);
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
        PerformanceAnalyzer::RenderingDrawCount += indexCount;
    }
}