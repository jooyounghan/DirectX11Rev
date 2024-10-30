#pragma once
#include "IUpdatable.h"
#include "Delegation.h"
#include "HeaderHelper.h"
#include "directxmath/DirectXMath.h"

#include <unordered_map>
#include <memory>

class Bone;
class BoneAsset;
class AnimationAsset;

typedef std::function<void()> AnimationEndedHandler;

template<typename T>
class StructuredBuffer;

//struct AnimationNotify
//{
//	float NotifyTime;
//};

class AnimationPlayer : public IUpdatable
{
public:
	AnimationPlayer();
	~AnimationPlayer();

protected:
	std::shared_ptr<AnimationAsset> AnimationAssetCached;
	std::shared_ptr<BoneAsset>		BoneAssetCached;
	size_t PlayCount = false;
	float PlayTime = 0.f;

public:
	void SetBoneAsset(const std::shared_ptr<BoneAsset>& BoneAssetIn);

protected:
	std::shared_ptr<StructuredBuffer<DirectX::XMMATRIX>> AnimationTransformationBuffer;
	MakeSmartPtrGetter(AnimationTransformationBuffer);

public:
	void PlayAnimation(const std::shared_ptr<AnimationAsset>& AnimationAssetIn, const size_t& PlayCountIn);

private:
	void StopAnimation(const bool& IsRaiseEvent = true);

//protected:
//	std::unordered_map<std::string, AnimationNotify> AnimNotifies;

public:
	Delegation<> AnimationEndedEvent;

private:
	bool IsPlaying();

public:
	virtual void Update(const float& DeltaTimeIn) override;

private:
	void UpdateBoneFromParent(Bone* Parent, Bone* Child);
};

