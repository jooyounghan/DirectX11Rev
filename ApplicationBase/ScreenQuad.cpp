#include "ScreenQuad.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

ScreenQuad::ScreenQuad()
{
	m_positions =
	{
		{-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {-1.f, -1.f, 1.f},
		{-1.f, -1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, -1.f, 1.f}
	};
	m_uvTextures =
	{
		{0.f, 0.f}, {1.f, 0.f}, {0.f, 1.f},
		{0.f, 1.f}, {1.f, 0.f}, {1.f, 1.f},
	};
	m_indices =
	{
		 0, 1, 2, 3, 4, 5
	};

	SetVertexBuffer(new ConstantBuffer(
		sizeof(XMFLOAT3), static_cast<UINT>(m_positions.size()), m_positions.data(), D3D11_BIND_VERTEX_BUFFER
	));
	SetUVBuffer(new ConstantBuffer(
		sizeof(XMFLOAT2), static_cast<UINT>(m_uvTextures.size()), m_uvTextures.data(), D3D11_BIND_VERTEX_BUFFER
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
	vector<ConstantBuffer*> vertexBuffers = GetVertexConstantBuffers();
	for (ConstantBuffer* vertexBuffers : vertexBuffers)
	{
		vertexBuffers->InitializeBuffer(device);
	}

	m_indexBuffer->InitializeBuffer(device);
}

vector<ConstantBuffer*> ScreenQuad::GetVertexConstantBuffers() const
{
	return vector<ConstantBuffer*>{  m_vertexBuffer, m_uvBuffer };
}

vector<ConstantBuffer*> ScreenQuad::GetVertexConstantBuffersForDepthTest() const
{
	return vector<ConstantBuffer*>();
}

vector<UINT> ScreenQuad::GetStrides() const
{
	return vector<UINT> {
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2)
	};
}

vector<UINT> ScreenQuad::GetOffsets() const
{
	return vector<UINT>{ 0, 0 };
}

std::vector<UINT> ScreenQuad::GetStridesForDepthTest() const
{
	return std::vector<UINT>();
}

std::vector<UINT> ScreenQuad::GetOffsetsForDepthTest() const
{
	return std::vector<UINT>();
}
