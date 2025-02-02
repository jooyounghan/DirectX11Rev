#include "StaticMeshAsset.h"

using namespace std;
using namespace DirectX;

const std::string StaticMeshAsset::DefaultSphereModelName = "sphere_model_Static";
const std::string StaticMeshAsset::DefaultCubeModelName = "cube_model_Static";

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

AMeshPartsData* StaticMeshAsset::AddMeshPartData(const uint32_t& lodLevel)
{
	if (m_staticMeshPartsPerLOD.find(lodLevel) == m_staticMeshPartsPerLOD.end())
	{
		m_staticMeshPartsPerLOD[lodLevel] = new StaticMeshPartsData();
	}
	return m_staticMeshPartsPerLOD[lodLevel];
}


AMeshPartsData* StaticMeshAsset::GetMeshPartData(const uint32_t& lodLevel) const
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
		const StaticMeshPartsData* const meshPartData = staticMeshPart.second;

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
		StaticMeshPartsData* staticMeshPartData = new StaticMeshPartsData();
		staticMeshPartData->Deserialize(fileIn);
		m_staticMeshPartsPerLOD.emplace(lodLevel, staticMeshPartData);
	}
}

void StaticMeshAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}
