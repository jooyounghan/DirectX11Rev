#include "DepthTestRenderer.h"

#include "ComponentPSOManager.h"
#include "GraphicsPSOObject.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"

#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"

#include "AMeshPartsData.h"

#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructuredBuffer.h"

#include "PerformanceAnalyzer.h"

using namespace std;

DepthTestRenderer::DepthTestRenderer(
    ID3D11DeviceContext* deviceContext,
    ComponentPSOManager* componentPsoManager,
    ID3D11Buffer* lightComponentEntityBuffer,
    ID3D11ShaderResourceView* const viewEntityStructuredBuffer,
    const D3D11_VIEWPORT* viewport,
    ID3D11DepthStencilView* depthStencilView
)
    : m_deviceContext(deviceContext),
    m_componentPsoManagerCached(componentPsoManager),
    m_lightComponentEntityBuffer(lightComponentEntityBuffer),
    m_viewEntityStructuredBuffer(viewEntityStructuredBuffer),
    m_viewport(viewport),
    m_depthStencilView(depthStencilView)
{
}

void DepthTestRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
    static GraphicsPSOObject* staticMeshGraphicsPSOObject
        = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::STATIC_MESH_DEPTH_TEST);

    if (staticMeshComponent != nullptr)
    {
        if (const StaticMeshAsset* staticMeshAsset = staticMeshComponent->GetStaticMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(NULL))
            {
                staticMeshGraphicsPSOObject->ApplyPSOObject(m_deviceContext);

                ID3D11RenderTargetView* nullRTV = nullptr;
                ID3D11DepthStencilView* nullDSV = nullptr;
                m_deviceContext->OMSetRenderTargets(0, &nullRTV, m_depthStencilView);
                m_deviceContext->RSSetViewports(1, m_viewport);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    m_lightComponentEntityBuffer,
                    staticMeshComponent->GetTransformationEntityBuffer().GetBuffer()
                };
                vector<ID3D11ShaderResourceView*> vsSRVs{
                    m_viewEntityStructuredBuffer,
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

void DepthTestRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
    static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
        = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_DEPTH_TEST);

    if (skeletalMeshComponent != nullptr)
    {
        if (const SkeletalMeshAsset* skeletalMeshAsset = skeletalMeshComponent->GetSkeletalMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(NULL))
            {
                skeletalMeshGraphicsPSOObject->ApplyPSOObject(m_deviceContext);

                ID3D11RenderTargetView* nullRTV = nullptr;
                ID3D11DepthStencilView* nullDSV = nullptr;
                m_deviceContext->OMSetRenderTargets(0, &nullRTV, m_depthStencilView);
                m_deviceContext->RSSetViewports(1, m_viewport);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    m_lightComponentEntityBuffer,
                    skeletalMeshComponent->GetTransformationEntityBuffer().GetBuffer()
                };

                vector<ID3D11ShaderResourceView*> vsSRVs{
                    m_viewEntityStructuredBuffer,
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

void DepthTestRenderer::Visit(CameraComponent* cameraComponent)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
    // Do Nothing
}

void DepthTestRenderer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
    // Do Nothing
}

void DepthTestRenderer::Visit(SpotLightComponent* spotLightComponent)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(PointLightComponent* pointLightComponent)
{
	// Do Nothing
}

void DepthTestRenderer::DepthTestMeshParts(const AMeshPartsData* const meshPartsData)
{
    const size_t& meshPartCount = meshPartsData->GetPartsCount();
    const vector<UINT>& indicesOffsets = meshPartsData->GetIndexOffsets();
    vector<ID3D11Buffer*> vertexBuffers = meshPartsData->GetVertexBuffersForDepthTest();
    const vector<UINT>& strides = meshPartsData->GetStridesForDepthTest();
    const vector<UINT>& verticesOffsets = meshPartsData->GetOffsetsForDepthTest();
    const UINT& totalIndicesCount = static_cast<UINT>(meshPartsData->GetIndices().size());

    for (size_t idx = 0; idx < meshPartCount; ++idx)
    {
        const UINT indexCount = (idx + 1 == meshPartCount ? totalIndicesCount : indicesOffsets[idx + 1]) - indicesOffsets[idx];

        m_deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
        m_deviceContext->IASetIndexBuffer(meshPartsData->GetIndexBuffer()->GetBuffer(), DXGI_FORMAT_R32_UINT, NULL);
        m_deviceContext->DrawIndexed(indexCount, indicesOffsets[idx], NULL);
        PerformanceAnalyzer::DepthTestDrawCount += indexCount;
    }
}
