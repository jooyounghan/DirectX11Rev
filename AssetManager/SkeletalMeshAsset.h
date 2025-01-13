#pragma once
#include "MeshAsset.h"
#include "SkeletalMeshPartsData.h"
#include "BoneAsset.h"

#include <map>

class SkeletalMeshAsset : public AMeshAsset
{
public:
	SkeletalMeshAsset() = default;
	SkeletalMeshAsset(const std::string& assetName);
	~SkeletalMeshAsset() override;

protected:
	std::map<uint32_t, SkeletalMeshPartsData*> m_skeletalMeshPartsPerLOD;

protected:
	std::string m_boneAssetName;
	const BoneAsset* m_boneAsset = nullptr;

public:
	inline const std::map<uint32_t, SkeletalMeshPartsData*>& GetSkeletalMeshPartsPerLOD() { return m_skeletalMeshPartsPerLOD; }

public:
	void SetBoneAsset(const BoneAsset* const boneAsset);
	void UpdateBoneAsset(IBoneProvider& provider);
	inline const BoneAsset* GetBoneAsset() const { return m_boneAsset; }

public:
	virtual size_t GetLODCount() const override;
	virtual MeshPartsData* AddMeshPartData(const uint32_t& lodLevel) override;
	virtual MeshPartsData* GetMeshPartData(const uint32_t& lodLevel) const override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;
};

class ISkeletalMeshProvider
{
public:
	virtual SkeletalMeshAsset* const GetSkeletalMeshAsset(
		const std::string& assetName
	) = 0;
};