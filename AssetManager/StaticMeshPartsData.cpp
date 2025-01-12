#include "StaticMeshPartsData.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

void StaticMeshPartsData::AddTangent(const float& x, const float& y, const float& z)
{
	m_tangents.emplace_back(XMFLOAT3(x, y, z));
}

void StaticMeshPartsData::Serialize(FILE* fileIn) const
{
	MeshPartsData::Serialize(fileIn);

	SerializeHelper::SerializeVectorContainer(m_tangents, fileIn);
}

void StaticMeshPartsData::Deserialize(FILE* fileIn)
{
	MeshPartsData::Deserialize(fileIn);
	m_tangents = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT3>>(fileIn);
}

vector<UINT> StaticMeshPartsData::GetStrides()
{
	return vector<UINT> {
		sizeof(XMFLOAT3),
			sizeof(XMFLOAT2),
			sizeof(XMFLOAT3),
			sizeof(XMFLOAT3)
	};
}

vector<UINT> StaticMeshPartsData::GetOffsets()
{
	return vector<UINT>{ 0, 0, 0, 0 };
}

void StaticMeshPartsData::InitializeGPUAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	ResetGPUAsset();
	m_vertexBuffers.clear();

	m_vertexBuffers.emplace_back(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(m_positions.size()), m_positions.data(), D3D11_BIND_VERTEX_BUFFER));
	m_vertexBuffers.emplace_back(new ConstantBuffer(sizeof(XMFLOAT2), static_cast<UINT>(m_uvTextures.size()), m_uvTextures.data(), D3D11_BIND_VERTEX_BUFFER));
	m_vertexBuffers.emplace_back(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(m_normals.size()), m_normals.data(), D3D11_BIND_VERTEX_BUFFER));
	m_vertexBuffers.emplace_back(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(m_tangents.size()), m_tangents.data(), D3D11_BIND_VERTEX_BUFFER));

	for (ConstantBuffer* constantBuffer : m_vertexBuffers)
	{
		constantBuffer->Initialize(device);
	}

	m_indexBuffer = new ConstantBuffer(sizeof(uint32_t), static_cast<UINT>(m_indices.size()), m_indices.data(), D3D11_BIND_INDEX_BUFFER);
	m_indexBuffer->Initialize(device);
}

void StaticMeshPartsData::Accept(IMeshPartsDataVisitor& visitor)
{
	visitor.Visit(this);
}
