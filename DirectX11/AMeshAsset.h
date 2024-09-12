#pragma once
#include "AAssetFile.h"
#include "Vertexable.h"
#include "AIndexable.h"
#include "DefineType.h"

class AMeshAsset : public AAssetFile, public AIndexable<uint32_t>
{
public:
	AMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~AMeshAsset();

public:
	Vertexable<XMFLOAT3> Positions;
	Vertexable<XMFLOAT2> UVTextures;
	Vertexable<XMFLOAT3> Normals;

public:
	virtual DXGI_FORMAT GetIndexFormat() override { return DXGI_FORMAT_R32_UINT; }

public:
	inline std::vector<ID3D11Buffer*> GetPositionBuffer() { return { Positions.GetVertexBuffer() }; }
	inline std::vector<UINT> GetPositionStride() { return { sizeof(XMFLOAT3)}; }
	inline std::vector<UINT> GetPositionOffset() { return { 0 }; }

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

public:
	virtual void Initialize();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;

protected:
	virtual void SerializeBaseMeshData(FILE* FileIn);
	virtual void DeserializeBaseMeshData(FILE* FileIn);
};

