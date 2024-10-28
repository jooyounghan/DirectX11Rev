#pragma once
#include "IUpdatable.h"

#include <memory>

class BoneAsset;
class AnimationAsset;

class AnimationPlayer : public IUpdatable
{
public:
	AnimationPlayer(const std::shared_ptr<BoneAsset>& BoneAssetIn);
	~AnimationPlayer();

protected:
	std::shared_ptr<AnimationAsset> AnimationAssetCached;
	std::shared_ptr<BoneAsset>		BoneAssetCached;
	size_t PlayCount = false;
	float PlayTime = 0.f;

public:
	void PlayAnimation(const std::shared_ptr<AnimationAsset>& AnimationAssetIn, const size_t& PlayCountIn);
	void StopAnimation();

private:
	bool IsPlaying();

public:
	virtual void Update(const float& DeltaTimeIn) override;
};

