#pragma once
#include "MeshAsset.h"

class StaticMeshPartData : public MeshPartsData
{
public:
	StaticMeshPartData() = default;

protected:
	std::vector<DirectX::XMFLOAT3> m_tangents;

public:
	void AddTangent(const float& x, const float& y, const float& z);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual std::vector<UINT> GetStrides() override;
	virtual std::vector<UINT> GetOffsets() override;

public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;
};

class StaticMeshAsset : public AMeshAsset
{
public:
	StaticMeshAsset() = default;
	StaticMeshAsset(const std::string& assetName);
	virtual ~StaticMeshAsset();

protected:
	std::map<uint32_t, StaticMeshPartData*> m_staticMeshPartsPerLOD;

public:
	virtual size_t GetLODCount() override;
	virtual MeshPartsData* GetMeshPartData(const uint32_t& lodLevel) override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;
};

class IStaticMeshProvider
{
public:
	virtual StaticMeshAsset* const GetStaticMeshAsset(
		const std::string& assetName
	) = 0;
};