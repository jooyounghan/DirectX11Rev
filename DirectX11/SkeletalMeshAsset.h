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


class SkeletalMeshAsset : public IMeshAsset
{
public:
	SkeletalMeshAsset();
	SkeletalMeshAsset(const std::string& AssetNameIn);
	virtual ~SkeletalMeshAsset();

public:
	std::vector<SkeletalVertex>		Vertices;

protected:
	VertexBuffer<SkeletalVertex>	VerticesBuffer;
	MakeGetter(VerticesBuffer);

public:
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn) override;
};

