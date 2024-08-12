#pragma once
#include "IAssetFile.h"
#include "Vertexable.h"
#include "IIndexable.h"
#include "DefineType.h"

class AMeshAsset : public IAssetFile, public IIndexable<uint32_t>
{
public:
	AMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~AMeshAsset();

public:
	Vertexable<SPosition3D> Positions;
	Vertexable<SCoordinate2D> UVTextures;
	Vertexable<SVector3D> Normals;
	Vertexable<SVector3D> Tangents;
	Vertexable<SVector3D> Bitangents;

public:
	virtual DXGI_FORMAT GetIndexFormat() override { return DXGI_FORMAT_R32_UINT; }

public:
	inline std::vector<ID3D11Buffer*> GetPositionBuffer() { return { Positions.GetVertexBuffer() }; }
	inline std::vector<UINT> GetPositionStride() { return { sizeof(SPosition3D)}; }
	inline std::vector<UINT> GetPositionOffset() { return { 0 }; }

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

public:
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) = 0;

protected:
	void SerializeBaseMeshData(FILE* FileIn);
	void DeserializeBaseMeshData(FILE* FileIn);
};

