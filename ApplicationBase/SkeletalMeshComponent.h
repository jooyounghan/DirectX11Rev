#pragma once
#include "AMeshComponent.h"
#include "AnimationPlayer.h"

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
	AnimationPlayer m_animationPlayer;

public:
	inline void SetSkeletalMeshName(const std::string& skeletalMeshName) { m_skeletalMeshName = skeletalMeshName; }
	inline const std::string& GetSkeletalMeshName() const { return m_skeletalMeshName; }

public:
	inline const SkeletalMeshAsset* GetSkeletalMetalAsset() const { return m_skeletalMeshAsset; }

public:
	void UpdateSkeletalMeshAsset(ISkeletalMeshProvider& provider);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

