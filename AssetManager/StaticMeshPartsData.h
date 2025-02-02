#pragma once
#include "AMeshPartsData.h"

class StaticMeshPartsData : public AMeshPartsData
{
public:
	StaticMeshPartsData() = default;
	~StaticMeshPartsData() override = default;

protected:
	ConstantBuffer* m_tangentBuffer = nullptr;
	std::vector<DirectX::XMFLOAT3> m_tangents;

public:
	inline ConstantBuffer* GetTangentBuffer() const { return m_tangentBuffer; }

public:
	void SetTangentBuffer(ConstantBuffer* tangentBuffers);

public:
	inline const std::vector<DirectX::XMFLOAT3>& GetTangents() { return m_tangents; }

public:
	void AddTangent(const float& x, const float& y, const float& z);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

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

protected:
	virtual void ResetMeshData() override;
};