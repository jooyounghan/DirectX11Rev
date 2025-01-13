
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace DirectX;


SkeletalMeshAsset::SkeletalMeshAsset(const string& assetName)
	: AMeshAsset(assetName)
{
}

SkeletalMeshAsset::~SkeletalMeshAsset()
{
	for (auto& skeletalMeshPartsPerLOD : m_skeletalMeshPartsPerLOD)
	{
		delete skeletalMeshPartsPerLOD.second;
	}
}

void SkeletalMeshAsset::SetBoneAsset(const BoneAsset* const boneAsset)
{
	m_boneAssetName = boneAsset->GetAssetName();
	m_boneAsset = boneAsset;
}

void SkeletalMeshAsset::UpdateBoneAsset(IBoneProvider& provider)
{
	m_boneAsset = provider.GetBoneAsset(m_boneAssetName);
}

size_t SkeletalMeshAsset::GetLODCount() const
{
	return m_skeletalMeshPartsPerLOD.size();
}

MeshPartsData* SkeletalMeshAsset::AddMeshPartData(const uint32_t& lodLevel)
{
	if (m_skeletalMeshPartsPerLOD.find(lodLevel) == m_skeletalMeshPartsPerLOD.end())
	{
		m_skeletalMeshPartsPerLOD[lodLevel] = new SkeletalMeshPartsData();
	}
	return m_skeletalMeshPartsPerLOD[lodLevel];
}

MeshPartsData* SkeletalMeshAsset::GetMeshPartData(const uint32_t& lodLevel) const
{
	if (m_skeletalMeshPartsPerLOD.find(lodLevel) != m_skeletalMeshPartsPerLOD.end())
	{
		return m_skeletalMeshPartsPerLOD.at(lodLevel);
	}
	return nullptr;
}

void SkeletalMeshAsset::Serialize(FILE* fileIn) const
{
	AMeshAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_skeletalMeshPartsPerLOD, fileIn);
	for (auto& skeletalMeshPart : m_skeletalMeshPartsPerLOD)
	{
		const uint32_t& lodLevel = skeletalMeshPart.first;
		const SkeletalMeshPartsData* const meshPartData = skeletalMeshPart.second;

		SerializeHelper::SerializeElement(lodLevel, fileIn);
		meshPartData->Serialize(fileIn);
	};

	SerializeHelper::SerializeString(m_boneAssetName, fileIn);

}

void SkeletalMeshAsset::Deserialize(FILE* fileIn)
{
	AMeshAsset::Deserialize(fileIn);

	const size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerSize; ++idx)
	{
		const uint32_t lodLevel = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
		SkeletalMeshPartsData* skeletalMeshPartData = new SkeletalMeshPartsData();
		skeletalMeshPartData->Deserialize(fileIn);
		m_skeletalMeshPartsPerLOD.emplace(lodLevel, skeletalMeshPartData);
	}

	m_boneAssetName = DeserializeHelper::DeserializeString(fileIn);
}

void SkeletalMeshAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}

