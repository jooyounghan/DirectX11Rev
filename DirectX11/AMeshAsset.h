#pragma once
#include "AAssetFile.h"
#include "Vertexable.h"
#include "Indexable.h"

#include "directxmath/DirectXMath.h"

class MaterialAsset;

class AMeshAsset : public AAssetFile
{
public:
	AMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~AMeshAsset();

public:
	std::vector<Vertexable<DirectX::XMFLOAT3>> PositionsPerLOD;
	std::vector<Vertexable<DirectX::XMFLOAT2>> UVTexturesPerLOD;
	std::vector<Vertexable<DirectX::XMFLOAT3>> NormalsPerLOD;
	std::vector<Indexable<uint32_t>> IndicesPerLOD;

public:
	std::vector<std::vector<UINT>> IndexCountsForPartPerLOD;
	std::vector<std::vector<UINT>> IndexOffsetsForPartPerLOD;
	std::vector<std::vector<UINT>> MaterialIndexPerLOD;

protected:
	std::vector<std::shared_ptr<MaterialAsset>> DefaultMaterialAssets;
	MakeSetterGetter(DefaultMaterialAssets);

protected:
	size_t LODCount = 0;
	MakeGetter(LODCount);

public:
	virtual void SetLODCount(const size_t& LODCountIn);

public:
	const std::vector<UINT> GetIndexCountsForPart(const size_t& LodCountIn);
	const std::vector<UINT> GetIndexOffsetsForPart(const size_t& LodCountIn);
	const std::vector<UINT> GetMaterialIndex(const size_t& LodCountIn);

public:
	ID3D11Buffer* GetIndexBuffer(const size_t& LODLevelIn = 0);
	UINT GetIndexCount(const size_t& LODLevelIn = 0);
	DXGI_FORMAT GetIndexFormat() { return DXGI_FORMAT_R32_UINT; }


public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers(const size_t& LODLevelIn = 0) = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

public:
	virtual void Initialize();

public:
	virtual void Serialize() = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;

protected:
	virtual void SerializeBaseMeshData(FILE* FileIn);
	virtual void DeserializeBaseMeshData(FILE* FileIn, AssetManager* AssetManagerIn);
};

