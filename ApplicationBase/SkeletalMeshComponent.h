#pragma once
#include "AMeshComponent.h"

class SkeletalMeshAsset;
class ISkeletalMeshProvider;

class SkeletalMeshComponent : public AMeshComponent
{
public:
	SkeletalMeshComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale
	);
	~SkeletalMeshComponent() override = default;

protected:
	std::string m_skeletalMeshName;
	const SkeletalMeshAsset* m_skeletalMeshAsset = nullptr;

public:
	inline void SetSkeletalMeshName(const std::string& skeletalMeshName) { m_skeletalMeshName = skeletalMeshName; }
	inline const std::string& GetSkeletalMeshName() { return m_skeletalMeshName; }

public:
	void UpdateSkeletalMeshAsset(ISkeletalMeshProvider& provider);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

