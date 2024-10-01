#pragma once
#include "ANBTMeshAsset.h"

constexpr const char* StaticMeshAssetOutPath = ".\\Assets\\StaticMesh\\";

class StaticMeshAsset : public ANBTMeshAsset
{
public:
	StaticMeshAsset(const std::string& AssetNameIn, bool LoadFromAsset);
	virtual ~StaticMeshAsset();

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers(const size_t& LODLevelIn) override;
	virtual std::vector<UINT> GetStrides() override;
	virtual std::vector<UINT> GetOffsets() override;

public:
	virtual void Initialize() override;

public:
	virtual std::string Serialize() override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};