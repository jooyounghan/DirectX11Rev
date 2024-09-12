#pragma once
#include "ANBTMeshAsset.h"

class BoneAsset;

class SkeletalMeshAsset : public ANBTMeshAsset
{
public:
	SkeletalMeshAsset(const std::string& AssetNameIn, bool LoadAsFile);
	virtual ~SkeletalMeshAsset();

protected:
	std::shared_ptr<BoneAsset> LinkedBoneAsset = nullptr;
	MakeSetterGetter(LinkedBoneAsset)

public:
	Vertexable<XMFLOAT4> BlendWeight;
	Vertexable<XMINT4> BlendIndex;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() override;
	virtual std::vector<UINT> GetStrides() override;
	virtual std::vector<UINT> GetOffsets() override;

public:
	virtual void Initialize() override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

