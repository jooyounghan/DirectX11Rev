#pragma once
#include "IMeshAsset.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DefineType.h"

struct SkeletalVertex
{
	SPosition3D		Position;
	SCoordinate2D	TextureCoord;
	SVector3D		Normal;
	SVector3D		Tangent;
	SVector3D		Bitangent;
	SVector4D		BlendWeight;
	SVector4D		BlendIndex;
};

class BoneAsset;

class SkeletalMeshAsset : public IMeshAsset
{
public:
	SkeletalMeshAsset();
	SkeletalMeshAsset(const std::string& AssetNameIn, bool LoadAsFile);
	virtual ~SkeletalMeshAsset();

public:
	std::vector<SkeletalVertex>		Vertices;

protected:
	VertexBuffer<SkeletalVertex>	VerticesBuffer;

protected:
	BoneAsset* LinkedBoneAsset = nullptr;
	std::string BoneAssetName = "";

public:
	const std::string& GetBoneAssetName() { return BoneAssetName; }
	void LinkBoneAsset(BoneAsset* BoneAssetIn);

public:
	virtual ID3D11Buffer* GetVertexBuffer() override { return VerticesBuffer.GetBuffer(); }
	virtual UINT GetVertexTypeSize() override { return sizeof(SkeletalVertex); }
	virtual UINT GetVertexCount() override { return static_cast<UINT>(Vertices.size()); }

public:
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;
};

