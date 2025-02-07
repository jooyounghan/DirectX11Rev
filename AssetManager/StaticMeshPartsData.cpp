#include "StaticMeshPartsData.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

std::vector<ConstantBuffer*> StaticMeshPartsData::GetVertexConstantBuffers() const
{
	return std::vector<ConstantBuffer*>
	{
		m_vertexBuffer,
		m_uvBuffer,
		m_normalBuffer
	};
}

std::vector<ConstantBuffer*> StaticMeshPartsData::GetVertexConstantBuffersForDepthTest() const
{
	return std::vector<ConstantBuffer*> { m_vertexBuffer };
}

vector<UINT> StaticMeshPartsData::GetStrides() const
{
	return vector<UINT> 
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3)
	};
}

vector<UINT> StaticMeshPartsData::GetOffsets() const
{
	return vector<UINT>{ 0, 0, 0 };
}

std::vector<UINT> StaticMeshPartsData::GetStridesForDepthTest() const
{
	return vector<UINT>{ sizeof(XMFLOAT3) };
}

std::vector<UINT> StaticMeshPartsData::GetOffsetsForDepthTest() const
{
	return std::vector<UINT>{ 0 };
}

void StaticMeshPartsData::Accept(IMeshPartsDataVisitor& visitor)
{
	visitor.Visit(this);
}
