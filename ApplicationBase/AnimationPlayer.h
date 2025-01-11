#pragma once
#include <unordered_map>
#include <memory>
#include <functional>

class Bone;
class BoneAsset;
class AnimationAsset;
class StructuredBuffer;

struct ID3D11Device;

//struct AnimationNotify
//{
//	float NotifyTime;
//};

class AnimationPlayer
{
public:
	AnimationPlayer() = default;
	~AnimationPlayer() = default;

protected:
	const AnimationAsset*	m_animationAssetCached;
	const BoneAsset*		m_boneAssetCached;
	size_t m_playCount = false;
	float m_playTime = 0.f;
//	std::unordered_map<std::string, AnimationNotify> AnimNotifies;

protected:
	std::vector<XMMATRIX>	m_animationTransformation;
	StructuredBuffer*		m_animationTransformationBuffer;

public:
	std::function<void()> m_animationEndedEvent;

public:
	void SetBoneAsset(ID3D11Device* const device, const BoneAsset* boneAsset);
	void PlayAnimation(const AnimationAsset* animationAsset, const size_t& playCountIn);
	void Update(const float& DeltaTimeIn);

private:
	bool IsPlaying();
	void StopAnimation(const bool& IsRaiseEvent = true);
	void UpdateBoneFromParent(const Bone* parentBone, const Bone* childBone);
};

