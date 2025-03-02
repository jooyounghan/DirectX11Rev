#include "PointLightDepthTestRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"

#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"

#include "AMeshPartsData.h"

#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "PointLightComponent.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructuredBuffer.h"

#include "PerformanceAnalyzer.h"

using namespace std;

PointLightDepthTestRenderer::PointLightDepthTestRenderer(
    ID3D11DeviceContext* deviceContext, 
    ComponentPSOManager* componentPsoManager, 
    ID3D11Buffer* viewEntityBufferCached, 
    const D3D11_VIEWPORT* viewportCached, 
    ID3D11DepthStencilView* depthStencilViewCached
)
    :ADepthTestRenderer(deviceContext, componentPsoManager),
    m_viewEntityBufferCached(viewEntityBufferCached),
    m_viewportCached(viewportCached),
    m_depthStencilViewCached(depthStencilViewCached)
{
}

void PointLightDepthTestRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
    static GraphicsPSOObject* staticMeshGraphicsPSOObject
        = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::STATIC_MESH_POINT_LIGHT_DEPTH_TEST);

    if (staticMeshComponent != nullptr)
    {
        if (const StaticMeshAsset* staticMeshAsset = staticMeshComponent->GetStaticMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(NULL))
            {
                staticMeshGraphicsPSOObject->ApplyPSOObject(m_deviceContext);

                ID3D11RenderTargetView* nullRTV = nullptr;
                ID3D11DepthStencilView* nullDSV = nullptr;
                m_deviceContext->OMSetRenderTargets(0, &nullRTV, m_depthStencilViewCached);
                m_deviceContext->RSSetViewports(1, m_viewportCached);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    staticMeshComponent->GetTransformationEntityBuffer().GetBuffer(),
                    m_viewEntityBufferCached
                };

                m_deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(vsConstantBuffers.size()), vsConstantBuffers.data());
                // ===================================================================

                DepthTestMeshParts(meshPartsData);

                m_deviceContext->OMSetRenderTargets(0, &nullRTV, nullDSV);
            }
        }
    }

}

void PointLightDepthTestRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
    static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
        = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_POINT_LIGHT_DEPTH_TEST);

    if (skeletalMeshComponent != nullptr)
    {
        if (const SkeletalMeshAsset* skeletalMeshAsset = skeletalMeshComponent->GetSkeletalMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(NULL))
            {
                skeletalMeshGraphicsPSOObject->ApplyPSOObject(m_deviceContext);

                ID3D11RenderTargetView* nullRTV = nullptr;
                ID3D11DepthStencilView* nullDSV = nullptr;
                m_deviceContext->OMSetRenderTargets(0, &nullRTV, m_depthStencilViewCached);
                m_deviceContext->RSSetViewports(1, m_viewportCached);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    skeletalMeshComponent->GetTransformationEntityBuffer().GetBuffer(),
                    m_viewEntityBufferCached
                };

                vector<ID3D11ShaderResourceView*> vsSRVs{
                    skeletalMeshComponent->GetAnimationPlayer()->GetBoneTransformationBuffer()->GetSRV()
                };

                m_deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(vsConstantBuffers.size()), vsConstantBuffers.data());
                m_deviceContext->VSSetShaderResources(0, static_cast<UINT>(vsSRVs.size()), vsSRVs.data());
                // ===================================================================

                DepthTestMeshParts(meshPartsData);

                m_deviceContext->OMSetRenderTargets(0, &nullRTV, nullDSV);
            }
        }
    }
}

void PointLightDepthTestRenderer::Visit(CameraComponent* cameraComponent)
{
    // Do Nothing
}

void PointLightDepthTestRenderer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
    // Do Nothing
}

void PointLightDepthTestRenderer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
    // Do Nothing
}

void PointLightDepthTestRenderer::Visit(SpotLightComponent* spotLightComponent)
{
    // Do Nothing
}

void PointLightDepthTestRenderer::Visit(PointLightComponent* pointLightComponent)
{
    // Do Nothing
}
