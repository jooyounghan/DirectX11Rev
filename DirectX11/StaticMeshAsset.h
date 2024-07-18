#pragma once
#include "IMeshAsset.h"
#include "VertexBuffer.h"
#include "DefineType.h"

struct StaticVertex
{
	SPosition3D		Position;
	SCoordinate2D	TextureCoord;
	SVector3D		Normal;
	SVector3D		Tangent;
	SVector3D		Bitangent;
};

class StaticMeshAsset : public IMeshAsset
{
public:
	StaticMeshAsset();
	StaticMeshAsset(const std::string& AssetNameIn);
	virtual ~StaticMeshAsset();

public:
	std::vector<StaticVertex>	Vertices;

protected:
	VertexBuffer<StaticVertex>	VerticesBuffer;

public:
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual ID3D11Buffer* GetVertexBuffer() override { return VerticesBuffer.GetBuffer(); }
	virtual UINT GetVertexTypeSize() override { return sizeof(StaticVertex); }
	virtual UINT GetVertexCount() override { return static_cast<UINT>(Vertices.size()); }

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn) override;
};