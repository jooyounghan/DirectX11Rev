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

vector<UINT> StaticMeshPartsData::GetStrides() const
{
	return vector<UINT> {
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

void StaticMeshPartsData::Accept(IMeshPartsDataVisitor& visitor)
{
	visitor.Visit(this);
}
