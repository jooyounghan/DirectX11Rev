#pragma once
#include "AMeshAsset.h"

class ANBTMeshAsset : public AMeshAsset
{
public:
	ANBTMeshAsset(const std::string& AssetNameIn, const std::string& AssetTypeIn);
	virtual ~ANBTMeshAsset();

public:
	std::vector<Vertexable<DirectX::XMFLOAT3>> TangentsPerLOD;

public:
	virtual void SetLODCount(const size_t& LODCountIn) override;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers(const size_t& LODLevelIn) = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

public:
	virtual void Initialize() override;

protected:
	virtual void SerializeBaseMeshData(FILE* FileIn) override;
	virtual void DeserializeBaseMeshData(FILE* FileIn) override;
};

