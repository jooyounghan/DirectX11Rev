#pragma once
#include "StaticMeshAsset.h"
#include "BoneAsset.h"

class SkeletalMeshPartData : public StaticMeshPartData
{
public:
	SkeletalMeshPartData() = default;

protected:
	std::vector<DirectX::XMFLOAT4> m_blendWeight;
	std::vector<DirectX::XMINT4> m_blendIndex;

public:
	void ResizeBlendProperties(const size_t& resizeCount);
	void SetBlendProperties(
		const size_t& vertexIndex,
		const int& boneIndex,
		const float& blendWeight
	);
public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual std::vector<UINT> GetStrides() override;
	virtual std::vector<UINT> GetOffsets() override;

public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;
};

class SkeletalMeshAsset : public AMeshAsset
{
public:
	SkeletalMeshAsset() = default;
	SkeletalMeshAsset(const std::string& assetName);
	virtual ~SkeletalMeshAsset();

protected:
	std::map<uint32_t, SkeletalMeshPartData*> m_skeletalMeshPartsPerLOD;

protected:
	std::string m_boneAssetName;
	const BoneAsset* m_boneAsset = nullptr;

public:
	void SetBoneAsset(const BoneAsset* const boneAsset);
	void UpdateBoneAsset(IBoneProvider& provider);

public:
	virtual size_t GetLODCount() override;
	virtual MeshPartsData* GetMeshPartData(const uint32_t& lodLevel) override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;

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