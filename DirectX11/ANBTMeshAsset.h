#pragma once
#include "AMeshAsset.h"

class ANBTMeshAsset : public AMeshAsset
{
public:
	ANBTMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~ANBTMeshAsset();

public:
	Vertexable<XMFLOAT3> Tangents;
	Vertexable<XMFLOAT3> Bitangents;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

public:
	virtual void Initialize() override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;

protected:
	virtual void SerializeBaseMeshData(FILE* FileIn) override;
	virtual void DeserializeBaseMeshData(FILE* FileIn) override;
};

