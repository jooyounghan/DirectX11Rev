#pragma once
#include "AMeshAsset.h"
#include "Vertexable.h"
#include "DefineType.h"

class BoneAsset;

class SkeletalMeshAsset : public AMeshAsset
{
public:
	SkeletalMeshAsset();
	SkeletalMeshAsset(const std::string& AssetNameIn, bool LoadAsFile);
	virtual ~SkeletalMeshAsset();

protected:
	BoneAsset* LinkedBoneAsset = nullptr;
	std::string BoneAssetName = "";

public:
	const std::string& GetBoneAssetName() { return BoneAssetName; }
	void LinkBoneAsset(BoneAsset* BoneAssetIn);

public:
	Vertexable<SVector4D> BlendWeight;
	Vertexable<SVector4D> BlendIndex;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() override;
	virtual std::vector<UINT> GetStrides() override;
	virtual std::vector<UINT> GetOffsets() override;

public:
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;
};

