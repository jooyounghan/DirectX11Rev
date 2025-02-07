#pragma once
#include "AMeshPartsData.h"

class StaticMeshPartsData : public AMeshPartsData
{
public:
	StaticMeshPartsData() = default;
	~StaticMeshPartsData() override = default;

public:
	virtual std::vector<ConstantBuffer*> GetVertexConstantBuffers() const override;
	virtual std::vector<ConstantBuffer*> GetVertexConstantBuffersForDepthTest() const override;

public:
	virtual std::vector<UINT> GetStrides() const override;
	virtual std::vector<UINT> GetOffsets() const override;
	virtual std::vector<UINT> GetStridesForDepthTest() const override;
	virtual std::vector<UINT> GetOffsetsForDepthTest() const override;

public:
	virtual void Accept(IMeshPartsDataVisitor& visitor) override;
};