#pragma once
#include "AAssetFile.h"
#include "Vertexable.h"
#include "Indexable.h"
#include "DefineType.h"

class AMeshAsset : public AAssetFile
{
public:
	AMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~AMeshAsset();

public:
	std::vector<Vertexable<XMFLOAT3>> PositionsPerLOD;
	std::vector<Vertexable<XMFLOAT2>> UVTexturesPerLOD;
	std::vector<Vertexable<XMFLOAT3>> NormalsPerLOD;
	std::vector<Indexable<uint32_t>> IndicesPerLOD;

protected:
	size_t LODCount = 0;
	MakeGetter(LODCount);

public:
	virtual void SetLODCount(const size_t& LODCountIn);

public:
	ID3D11Buffer* GetIndexBuffer(const size_t& LODLevelIn = 0);
	UINT GetIndexCount(const size_t& LODLevelIn = 0);
	DXGI_FORMAT GetIndexFormat() { return DXGI_FORMAT_R32_UINT; }

public:
	std::vector<ID3D11Buffer*> GetPositionBuffer(const size_t& LODLevelIn = 0);
	inline std::vector<UINT> GetPositionStride() { return { sizeof(XMFLOAT3)}; }
	inline std::vector<UINT> GetPositionOffset() { return { 0 }; }

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers(const size_t& LODLevelIn = 0) = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

public:
	virtual void Initialize();

public:
	virtual std::string Serialize() = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;

protected:
	virtual void SerializeBaseMeshData(FILE* FileIn);
	virtual void DeserializeBaseMeshData(FILE* FileIn);
};

