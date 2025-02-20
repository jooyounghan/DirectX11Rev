#pragma once
#include "AMeshComponent.h"
#include "AnimationPlayer.h"

class SkeletalMeshAsset;
class ISkeletalMeshProvider;

enum class ESkeletalMeshComponentUpdateOption
{
	ANIMATION_ENTITY = EComponentUpdateOption::COMPONENT_UPDATE_OPTION_OFFSET,
	CAMERA_UPDATE_OPTION_OFFSET
};

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
	AnimationPlayer* m_animationPlayer = nullptr;

public:
	inline void SetSkeletalMeshName(const std::string& skeletalMeshName) { m_skeletalMeshName = skeletalMeshName; }
	inline const std::string& GetSkeletalMeshName() const { return m_skeletalMeshName; }
	inline const SkeletalMeshAsset* GetSkeletalMetalAsset() const { return m_skeletalMeshAsset; }
	inline AnimationPlayer* GetAnimationPlayer() const { return m_animationPlayer; }
	void SetAnimationPlayer(AnimationPlayer* animationPlayer);

public:
	void UpdateSkeletalMeshAsset(ISkeletalMeshProvider& provider);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

