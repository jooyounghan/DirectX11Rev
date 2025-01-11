#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include <DirectXMath.h>

class Bone;
class BoneAsset;
class AnimationAsset;
class StructuredBuffer;

struct ID3D11Device;
struct ID3D11DeviceContext;

//struct AnimationNotify
//{
//	float NotifyTime;
//};

class AnimationPlayer
{
public:
	AnimationPlayer(const BoneAsset* boneAsset);
	~AnimationPlayer() = default;

protected:
	const BoneAsset*		m_boneAssetCached;
	const AnimationAsset*	m_animationAssetCached;
	size_t m_playCount = false;
	float m_playTime = 0.f;
//	std::unordered_map<std::string, AnimationNotify> AnimNotifies;

protected:
	std::vector<DirectX::XMMATRIX>	m_boneTransformation;
	StructuredBuffer* m_boneTransformationBuffer = nullptr;

public:
	std::function<void()> m_animationEndedEvent;

public:
	void PlayAnimation(const AnimationAsset* animationAsset, const size_t& playCountIn);
	inline StructuredBuffer* GetBoneTransformationBuffer() const { return m_boneTransformationBuffer; }

private:
	bool IsPlaying();
	void StopAnimation();
	void UpdateBoneFromParent(const Bone* parentBone, const Bone* childBone);

public:
	void InitAnimationPlayer(ID3D11Device* const device);
	void UpdateAnimationPlayer(ID3D11DeviceContext* const deviceContext, const float& deltaTime);
};

