
#include "MeshAsset.h"
#include "ModelMaterialAsset.h"

using namespace std;
using namespace DirectX;

AMeshAsset::AMeshAsset(const string& assetName)
	: AAsset(assetName)
{
}

void AMeshAsset::UpdateDefaultMaterialAsset(IModelMaterialProvider& provider)
{
	m_defaultMaterials.clear();
	for (const string& defaultMaterialName : m_defaultMaterialNames)
	{
		m_defaultMaterials.emplace_back(provider.GetModelMaterialAsset(defaultMaterialName));
	}
}

void AMeshAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_defaultMaterialNames, fileIn);
	for (const string& defaultMaterialName : m_defaultMaterialNames)
	{
		SerializeHelper::SerializeString(defaultMaterialName, fileIn);
	}
}

void AMeshAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	const size_t containerCount = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerCount; ++idx)
	{
		m_defaultMaterialNames.emplace_back(DeserializeHelper::DeserializeString(fileIn));
	}
}
