#pragma once
#include "IMeshAsset.h"
#include "Vertexable.h"
#include "DefineType.h"

struct SkeletalVertex
{
	SPosition3D		Position;
	SCoordinate2D	UVTexture;
	SVector3D		Normal;
	SVector3D		Tangent;
	SVector3D		Bitangent;
	SVector4D		BlendWeight;
	SVector4D		BlendIndex;
};

class BoneAsset;

class SkeletalMeshAsset : public IMeshAsset, public Vertexable<SkeletalVertex>
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
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual void GetVertexInformation(ID3D11Buffer*& RefVertexBuffer, UINT& RefVertexTypeSize) override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;
};

