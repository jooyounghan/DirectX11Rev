#pragma once
#include "StaticMeshAsset.h"
#include "BoneAsset.h"

class SkeletalMeshPartData : public StaticMeshPartData
{
protected:
	std::vector<DirectX::XMFLOAT4> m_blendWeight;
	std::vector<DirectX::XMINT4> m_blendIndex;

public:
	void AppendBlendProperties(const size_t& appendCount);
	void SetBlendProperties(
		const size_t& vertexIndex,
		const int& boneIndex,
		const float& blendWeight
	);
public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class SkeletalMeshAsset : public AMeshAsset
{
public:
	SkeletalMeshAsset() = default;
	SkeletalMeshAsset(const std::string& assetName);
	virtual ~SkeletalMeshAsset();

protected:
	std::map<uint32_t, std::shared_ptr<SkeletalMeshPartData>> m_skeletalMeshPartsPerLOD;

protected:
	std::string m_boneAssetName;
	std::shared_ptr<BoneAsset> m_boneAsset;

public:
	void SetBoneAsset(
		const std::shared_ptr<BoneAsset>& boneAsset
	);

public:
	void UpdateBoneAsset(
		IBoneProvider& provider
	);

public:
	virtual size_t GetLODCount() override;
	virtual std::shared_ptr<MeshPartsData> GetMeshPartData(const uint32_t& lodLevel) override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class ISkeletalMeshProvider
{
public:
	virtual std::shared_ptr<SkeletalMeshAsset> GetSkeletalMeshAsset(
		const std::string& assetName
	) = 0;
};