#pragma once
#include "AMeshAsset.h"

class ANBTMeshAsset : public AMeshAsset
{
public:
	ANBTMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~ANBTMeshAsset();

public:
	std::vector<Vertexable<XMFLOAT3>> TangentsPerLOD;
	std::vector<Vertexable<XMFLOAT3>> BitangentsPerLOD;

public:
	virtual void SetLODCount(const size_t& LODCountIn) override;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers(const size_t& LODLevelIn) = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

public:
	virtual void Initialize() override;

public:
	virtual std::string Serialize() = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;

protected:
	virtual void SerializeBaseMeshData(FILE* FileIn) override;
	virtual void DeserializeBaseMeshData(FILE* FileIn) override;
};

