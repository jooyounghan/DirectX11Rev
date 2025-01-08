#include "pch.h"
#include "StaticMeshAsset.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;

void StaticMeshPartData::AddTangent(const float& x, const float& y, const float& z)
{
	m_tangents.emplace_back(XMFLOAT3(x, y, z));
}

void StaticMeshPartData::Serialize(FILE* fileIn) const
{
	MeshPartsData::Serialize(fileIn);

	SerializeHelper::SerializeSequenceContainer(m_tangents, fileIn);
}

void StaticMeshPartData::Deserialize(FILE* fileIn)
{
	MeshPartsData::Deserialize(fileIn);

	m_tangents = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT3>>(fileIn);
}

vector<UINT> StaticMeshPartData::GetStrides()
{
	return vector<UINT> {
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3)
	};
}

vector<UINT> StaticMeshPartData::GetOffsets()
{
	return vector<UINT>{ 0, 0, 0, 0 };
}

void StaticMeshPartData::InitializeGPUAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
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



StaticMeshAsset::StaticMeshAsset(const string& assetName)
	: AMeshAsset(assetName)
{

}

StaticMeshAsset::~StaticMeshAsset() 
{
	for (auto& staticMeshPartsPerLOD : m_staticMeshPartsPerLOD)
	{
		delete staticMeshPartsPerLOD.second;
	}
}

size_t StaticMeshAsset::GetLODCount() const
{
	return m_staticMeshPartsPerLOD.size();
}

MeshPartsData* StaticMeshAsset::AddMeshPartData(const uint32_t& lodLevel)
{
	if (m_staticMeshPartsPerLOD.find(lodLevel) != m_staticMeshPartsPerLOD.end())
	{
		delete m_staticMeshPartsPerLOD[lodLevel];
	}
	m_staticMeshPartsPerLOD[lodLevel] = new StaticMeshPartData();
	return m_staticMeshPartsPerLOD[lodLevel];
}


MeshPartsData* StaticMeshAsset::GetMeshPartData(const uint32_t& lodLevel) const
{
	if (m_staticMeshPartsPerLOD.find(lodLevel) != m_staticMeshPartsPerLOD.end())
	{
		return m_staticMeshPartsPerLOD.at(lodLevel);
	}
	return nullptr;
}

void StaticMeshAsset::Serialize(FILE* fileIn) const
{
	AMeshAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_staticMeshPartsPerLOD, fileIn);
	for (auto& staticMeshPart : m_staticMeshPartsPerLOD)
	{
		const uint32_t& lodLevel = staticMeshPart.first;
		const StaticMeshPartData* const meshPartData = staticMeshPart.second;

		SerializeHelper::SerializeElement(lodLevel, fileIn);
		meshPartData->Serialize(fileIn);
	}
}

void StaticMeshAsset::Deserialize(FILE* fileIn)
{
	AMeshAsset::Deserialize(fileIn);
	
	const size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerSize; ++idx)
	{
		const uint32_t lodLevel = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
		StaticMeshPartData* staticMeshPartData = new StaticMeshPartData();
		staticMeshPartData->Deserialize(fileIn);
		m_staticMeshPartsPerLOD.emplace(lodLevel, staticMeshPartData);
	}
}

void StaticMeshAsset::InitializeGPUAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	for (auto& staticMeshPartPair : m_staticMeshPartsPerLOD)
	{
		staticMeshPartPair.second->InitializeGPUAsset(device, deviceContext);
	}
}

void StaticMeshAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}
