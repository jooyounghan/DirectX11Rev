#pragma once
#include "AMeshAsset.h"

class StaticMeshAsset : public AMeshAsset
{
public:
	StaticMeshAsset();
	StaticMeshAsset(const std::string& AssetNameIn, bool LoadAsFile);
	virtual ~StaticMeshAsset();

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() override;
	virtual std::vector<UINT> GetStrides() override;
	virtual std::vector<UINT> GetOffsets() override;

public:
	virtual void Initialize(ID3D11Device* DeviceIn) override;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;
};