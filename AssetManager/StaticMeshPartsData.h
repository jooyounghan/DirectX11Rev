#pragma once
#include "MeshPartsData.h"

class StaticMeshPartsData : public MeshPartsData
{
public:
	StaticMeshPartsData() = default;
	virtual ~StaticMeshPartsData() override = default;

protected:
	std::vector<DirectX::XMFLOAT3> m_tangents;

public:
	inline const std::vector<DirectX::XMFLOAT3>& GetTangents() { return m_tangents; }

public:
	void AddTangent(const float& x, const float& y, const float& z);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual std::vector<UINT> GetStrides() const override;
	virtual std::vector<UINT> GetOffsets() const override;

public:
	virtual void Accept(IMeshPartsDataVisitor& visitor) override;
};