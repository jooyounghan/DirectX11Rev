#pragma once
#include "StaticMeshPartsData.h"

class SkeletalMeshPartsData : public StaticMeshPartsData
{
public:
	SkeletalMeshPartsData() = default;
	~SkeletalMeshPartsData() override = default;

protected:
	std::vector<DirectX::XMFLOAT4> m_blendWeight;
	std::vector<DirectX::XMINT4> m_blendIndex;

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