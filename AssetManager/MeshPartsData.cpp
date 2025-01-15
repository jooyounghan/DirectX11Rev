#include "MeshPartsData.h"

using namespace std;
using namespace DirectX;

const DirectX::XMFLOAT3& MeshPartsData::GetPosition(const size_t& index) const
{
	if (m_positions.size() > index)
	{
		return m_positions[index];
	}
	return m_emptyFloat3;
}

const DirectX::XMFLOAT2& MeshPartsData::GetUVTextureCoord(const size_t& index) const
{
	if (m_uvTextures.size() > index)
	{
		return m_uvTextures[index];
	}
	return m_emptyFloat2;
}

const DirectX::XMFLOAT3& MeshPartsData::GetNormal(const size_t& index) const
{
	if (m_normals.size() > index)
	{
		return m_normals[index];
	}
	return m_emptyFloat3;
}

void MeshPartsData::AddPosition(const float& x, const float& y, const float& z)
{
	m_positions.emplace_back(XMFLOAT3(x, y, z));
}

void MeshPartsData::AddUVTexture(const float& u, const float& v)
{
	m_uvTextures.emplace_back(XMFLOAT2(u, v));
}

void MeshPartsData::AddNormal(const float& x, const float& y, const float& z)
{
	m_normals.emplace_back(XMFLOAT3(x, y, z));
}

void MeshPartsData::AddPartOffsets(const uint32_t& vertexOffset, const uint32_t& indexOffset)
{
	m_vertexPartOffsets.emplace_back(vertexOffset);
	m_indexPartOffsets.emplace_back(indexOffset);
}


void MeshPartsData::AddIndex(const uint32_t& offset, const uint32_t index)
{
	m_indices.emplace_back(offset + index);
}

void MeshPartsData::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeVectorContainer(m_positions, fileIn);
	SerializeHelper::SerializeVectorContainer(m_uvTextures, fileIn);
	SerializeHelper::SerializeVectorContainer(m_normals, fileIn);
	SerializeHelper::SerializeVectorContainer(m_indices, fileIn);
	SerializeHelper::SerializeVectorContainer(m_vertexPartOffsets, fileIn);
	SerializeHelper::SerializeVectorContainer(m_indexPartOffsets, fileIn);
}

void MeshPartsData::Deserialize(FILE* fileIn)
{
	m_positions = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT3>>(fileIn);
	m_uvTextures = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT2>>(fileIn);
	m_normals = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT3>>(fileIn);
	m_indices = DeserializeHelper::DeserializeVectorContainer<vector<uint32_t>>(fileIn);
	m_vertexPartOffsets = DeserializeHelper::DeserializeVectorContainer<vector<uint32_t>>(fileIn);
	m_indexPartOffsets = DeserializeHelper::DeserializeVectorContainer<vector<uint32_t>>(fileIn);
}