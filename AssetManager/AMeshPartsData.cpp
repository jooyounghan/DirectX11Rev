#include "AMeshPartsData.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

void AMeshPartsData::SetNormalBuffer(ConstantBuffer* normalBuffer)
{
	if (m_normalBuffer) delete m_normalBuffer;
	m_normalBuffer = normalBuffer;
}

const XMFLOAT3& AMeshPartsData::GetNormal(const size_t& index) const
{
	if (m_normals.size() > index)
	{
		return m_normals[index];
	}
	return EmptyFloat3;
}

void AMeshPartsData::AddNormal(const float& x, const float& y, const float& z)
{
	m_normals.emplace_back(XMFLOAT3(x, y, z));
}

void AMeshPartsData::AddPartOffsets(const uint32_t& vertexOffset, const uint32_t& indexOffset)
{
	m_vertexPartOffsets.emplace_back(vertexOffset);
	m_indexPartOffsets.emplace_back(indexOffset);
}


void AMeshPartsData::AddIndex(const uint32_t& offset, const uint32_t index)
{
	m_indices.emplace_back(offset + index);
}

void AMeshPartsData::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeVectorContainer(m_positions, fileIn);
	SerializeHelper::SerializeVectorContainer(m_uvTextures, fileIn);
	SerializeHelper::SerializeVectorContainer(m_normals, fileIn);
	SerializeHelper::SerializeVectorContainer(m_indices, fileIn);
	SerializeHelper::SerializeVectorContainer(m_vertexPartOffsets, fileIn);
	SerializeHelper::SerializeVectorContainer(m_indexPartOffsets, fileIn);
}

void AMeshPartsData::Deserialize(FILE* fileIn)
{
	m_positions = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT3>>(fileIn);
	m_uvTextures = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT2>>(fileIn);
	m_normals = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT3>>(fileIn);
	m_indices = DeserializeHelper::DeserializeVectorContainer<vector<uint32_t>>(fileIn);
	m_vertexPartOffsets = DeserializeHelper::DeserializeVectorContainer<vector<uint32_t>>(fileIn);
	m_indexPartOffsets = DeserializeHelper::DeserializeVectorContainer<vector<uint32_t>>(fileIn);
}

void AMeshPartsData::ResetMeshData()
{
	AMeshData::ResetMeshData();
	if (m_normalBuffer) delete m_normalBuffer;
}
