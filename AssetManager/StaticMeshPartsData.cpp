#include "StaticMeshPartsData.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

void StaticMeshPartsData::SetTangentBuffer(ConstantBuffer* tangentBuffers)
{
	if (m_tangentBuffer) delete m_tangentBuffer;
	m_tangentBuffer = tangentBuffers;
}

void StaticMeshPartsData::AddTangent(const float& x, const float& y, const float& z)
{
	m_tangents.emplace_back(XMFLOAT3(x, y, z));
}

void StaticMeshPartsData::Serialize(FILE* fileIn) const
{
	AMeshPartsData::Serialize(fileIn);

	SerializeHelper::SerializeVectorContainer(m_tangents, fileIn);
}

void StaticMeshPartsData::Deserialize(FILE* fileIn)
{
	AMeshPartsData::Deserialize(fileIn);
	m_tangents = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT3>>(fileIn);
}

std::vector<ConstantBuffer*> StaticMeshPartsData::GetVertexConstantBuffers() const
{
	return std::vector<ConstantBuffer*>
	{
		m_vertexBuffer,
		m_uvBuffer,
		m_normalBuffer,
		m_tangentBuffer
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
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3)
	};
}

vector<UINT> StaticMeshPartsData::GetOffsets() const
{
	return vector<UINT>{ 0, 0, 0, 0 };
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

void StaticMeshPartsData::ResetMeshData()
{
	AMeshPartsData::ResetMeshData();
	if (m_tangentBuffer) delete m_tangentBuffer;
}
