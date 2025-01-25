#include "ScreenQuad.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

ScreenQuad::ScreenQuad()
{
	AddVertexBuffer(new ConstantBuffer(
		sizeof(XMFLOAT3), static_cast<UINT>(m_screenQuadPositions.size()), m_screenQuadPositions.data(), D3D11_BIND_VERTEX_BUFFER
	));
	AddVertexBuffer(new ConstantBuffer(
		sizeof(XMFLOAT2), static_cast<UINT>(m_screenQuadUVs.size()), m_screenQuadUVs.data(), D3D11_BIND_VERTEX_BUFFER
	));
	SetIndexBuffer(new ConstantBuffer(
		sizeof(uint32_t), static_cast<UINT>(m_indices.size()), m_indices.data(), D3D11_BIND_INDEX_BUFFER
	));
}

ScreenQuad* ScreenQuad::GetInstance()
{
	static ScreenQuad screenQuad;
	return &screenQuad;
}

void ScreenQuad::InitScreenQuad(ID3D11Device* device)
{
	for (ConstantBuffer* vertexBuffers : m_vertexBuffers)
	{
		D3D11_SUBRESOURCE_DATA vertexSubresourceData = vertexBuffers->GetSubResourceData();
		vertexBuffers->InitializeBuffer(device, &vertexSubresourceData);
	}

	D3D11_SUBRESOURCE_DATA screenQuadIndexSubresourceData = m_indexBuffer->GetSubResourceData();
	m_indexBuffer->InitializeBuffer(device, &screenQuadIndexSubresourceData);
}

std::vector<UINT> ScreenQuad::GetStrides() const
{
	return vector<UINT> {
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2)
	};
}

std::vector<UINT> ScreenQuad::GetOffsets() const
{
	return vector<UINT>{ 0, 0 };
}
