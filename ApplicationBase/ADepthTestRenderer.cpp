#include "ADepthTestRenderer.h"
#include "AMeshPartsData.h"
#include "ConstantBuffer.h"
#include "PerformanceAnalyzer.h"

using namespace std;

ADepthTestRenderer::ADepthTestRenderer(ID3D11DeviceContext* deviceContext, ComponentPSOManager* componentPsoManager)
    : m_deviceContext(deviceContext),
    m_componentPsoManagerCached(componentPsoManager)
{
}

void ADepthTestRenderer::DepthTestMeshParts(const AMeshPartsData* const meshPartsData)
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
