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

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructuredBuffer.h"

#include "PerformanceAnalyzer.h"

using namespace std;

DepthTestRenderer::DepthTestRenderer(
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager, 
    ID3D11Buffer* viewProjMatrix,
    D3D11_VIEWPORT* viewport,
    Texture2DInstance<SRVOption, DSVOption>* depthStencilViewTexture
)
    : m_deviceContextAddress(deviceContextAddress),
    m_componentPsoManagerCached(componentPsoManager),
    m_viewProjMatrix(viewProjMatrix),
    m_viewport(viewport),
    m_depthStencilViewTexture(depthStencilViewTexture)
{
}

void DepthTestRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
    static GraphicsPSOObject* staticMeshGraphicsPSOObject
        = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::STATIC_MESH_DEPTH_TEST);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;

    if (staticMeshComponent != nullptr)
    {
        if (const StaticMeshAsset* staticMeshAsset = staticMeshComponent->GetStaticMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = staticMeshAsset->GetMeshPartData(NULL))
            {
                staticMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);

                ID3D11RenderTargetView* nullRTV = nullptr;
                deviceContext->OMSetRenderTargets(0, &nullRTV, m_depthStencilViewTexture->GetDSV());
                deviceContext->RSSetViewports(1, m_viewport);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    m_viewProjMatrix,
                    staticMeshComponent->GetTransformationBuffer()->GetBuffer()
                };
                deviceContext->VSSetConstantBuffers(0, 2, vsConstantBuffers.data());
                // ===================================================================

                DepthTestMeshParts(deviceContext, meshPartsData);
            }
        }
    }

}

void DepthTestRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
    static GraphicsPSOObject* skeletalMeshGraphicsPSOObject
        = m_componentPsoManagerCached->GetGraphicsPSOObject(EComopnentGraphicsPSOObject::SKELETAL_MESH_DEPTH_TEST);

    ID3D11DeviceContext* const deviceContext = *m_deviceContextAddress;

    if (skeletalMeshComponent != nullptr)
    {
        if (const SkeletalMeshAsset* skeletalMeshAsset = skeletalMeshComponent->GetSkeletalMetalAsset())
        {
            if (AMeshPartsData* meshPartsData = skeletalMeshAsset->GetMeshPartData(NULL))
            {
                skeletalMeshGraphicsPSOObject->ApplyPSOObject(deviceContext);

                ID3D11RenderTargetView* nullRTV = nullptr;
                deviceContext->OMSetRenderTargets(0, &nullRTV, m_depthStencilViewTexture->GetDSV());
                deviceContext->RSSetViewports(1, m_viewport);

                // =============================== VS ===============================
                vector<ID3D11Buffer*> vsConstantBuffers{
                    m_viewProjMatrix,
                    skeletalMeshComponent->GetTransformationBuffer()->GetBuffer()
                };
                vector<ID3D11ShaderResourceView*> vsSRVs{ skeletalMeshComponent->GetAnimationPlayer()->GetBoneTransformationBuffer()->GetSRV() };

                deviceContext->VSSetConstantBuffers(0, 2, vsConstantBuffers.data());
                deviceContext->VSSetShaderResources(0, 1, vsSRVs.data());
                // ===================================================================

                DepthTestMeshParts(deviceContext, meshPartsData);
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
    sphereCollisionComponent->SetRenderable(false);
}

void DepthTestRenderer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
    orientedBoxCollisionComponent->SetRenderable(false);
}

void DepthTestRenderer::Visit(SpotLightComponent* spotLightComponent)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(PointLightComponent* pointLightComponent)
{
	// Do Nothing
}

void DepthTestRenderer::DepthTestMeshParts(ID3D11DeviceContext* const deviceContext, const AMeshPartsData* const meshPartsData)
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

        deviceContext->IASetVertexBuffers(0, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data(), strides.data(), verticesOffsets.data());
        deviceContext->IASetIndexBuffer(meshPartsData->GetIndexBuffer()->GetBuffer(), DXGI_FORMAT_R32_UINT, NULL);
        deviceContext->DrawIndexed(indexCount, indicesOffsets[idx], NULL);
        PerformanceAnalyzer::DrawForDepthTestCount += indexCount;
    }
}
