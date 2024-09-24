#pragma once
#include "AMeshAsset.h"
#include <mutex>

class BaseMeshAsset : public AMeshAsset
{
protected:
	BaseMeshAsset(const std::string& AssetNameIn);
	virtual ~BaseMeshAsset();
	MakeSingleton(BaseMeshAsset);

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers(const size_t& LODLevelIn = 0) override final;
	virtual std::vector<UINT> GetStrides() override final;
	virtual std::vector<UINT> GetOffsets() override final;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override final {/* Base Mesh Does Not Serialize*/};
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override final {/* Base Mesh Does Not Deserialize*/};
};

