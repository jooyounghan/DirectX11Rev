#pragma once
#include "MeshPartsData.h"

class StaticMeshPartsData : public MeshPartsData
{
public:
	StaticMeshPartsData() = default;
	~StaticMeshPartsData() override = default;

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

public:
	virtual void Accept(IMeshPartsDataVisitor& visitor) override;
};