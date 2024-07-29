#pragma once
#include "IMeshAsset.h"
#include "Vertexable.h"
#include "DefineType.h"

struct StaticVertex
{
	SPosition3D		Position;
	SCoordinate2D	UVTexture;
	SVector3D		Normal;
	SVector3D		Tangent;
	SVector3D		Bitangent;
};

class StaticMeshAsset : public IMeshAsset, public Vertexable<StaticVertex>
{
public:
	StaticMeshAsset();
	StaticMeshAsset(const std::string& AssetNameIn, bool LoadAsFile);
	virtual ~StaticMeshAsset();

public:
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual void GetVertexInformation(ID3D11Buffer*& RefVertexBuffer, UINT& RefVertexTypeSize) override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;
};