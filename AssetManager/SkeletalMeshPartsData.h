#pragma once
#include "StaticMeshPartsData.h"

class SkeletalMeshPartsData : public StaticMeshPartsData
{
public:
	SkeletalMeshPartsData() = default;
	~SkeletalMeshPartsData() override = default;

protected:
	ConstantBuffer* m_blendWeightBuffer = nullptr;
	ConstantBuffer* m_blendIndexBuffer = nullptr;
	std::vector<DirectX::XMFLOAT4> m_blendWeights;
	std::vector<DirectX::XMINT4> m_blendIndex;

public:
	inline ConstantBuffer* GetBlendWeightBuffer() const { return m_blendWeightBuffer; }
	inline ConstantBuffer* GetBlendIndexBuffer() const { return m_blendIndexBuffer; }

public:
	void SetBlendWeightBuffer(ConstantBuffer* blendWeightBuffer);
	void SetBlendIndexBuffer(ConstantBuffer* blendIndexBuffer);

public:
	inline const std::vector<DirectX::XMFLOAT4>& GetBlendWeights() { return m_blendWeights; }
	inline const std::vector<DirectX::XMINT4>& GetBlendIndices() { return m_blendIndex; }


public:
	void ResizeBlendProperties(const size_t& resizeCount);
	void SetBlendProperties(
		const size_t& vertexIndex,
		const int& boneIndex,
		const float& blendWeight
	);
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