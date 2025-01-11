#include "AnimationPlayer.h"

#include "BoneAsset.h"
#include "AnimationAsset.h"

#include "StructuredBuffer.h"

using namespace std;
using namespace DirectX;

void AnimationPlayer::SetBoneAsset(ID3D11Device* const device, const BoneAsset* boneAsset)
{
	StopAnimation(false);

	if (m_animationTransformationBuffer != nullptr)
	{
		m_animationTransformationBuffer->GetBuffer()->Release();
		delete m_animationTransformationBuffer;
	}

	m_boneAssetCached = boneAsset;

	const size_t boneCounts = m_boneAssetCached->GetBones().size();

	m_animationTransformation.clear();
	m_animationTransformation.resize(boneCounts, XMMatrixIdentity());
	m_animationTransformationBuffer = new StructuredBuffer(sizeof(XMMATRIX), boneCounts);
	m_animationTransformationBuffer->Initialize(device);
}

void AnimationPlayer::PlayAnimation(const AnimationAsset* animationAssetIn, const size_t& playCountIn)
{
	if (m_boneAssetCached != nullptr)
	{
		m_animationAssetCached = animationAssetIn;
		m_playCount = playCountIn;
		m_playTime = 0.f;
	}
}

void AnimationPlayer::StopAnimation(const bool& IsRaiseEvent)
{
	m_animationAssetCached = nullptr;
	m_playCount = 0;
	m_playTime = 0.f;

	if (IsRaiseEvent) m_animationEndedEvent();
}

inline bool AnimationPlayer::IsPlaying() { return (m_animationAssetCached != nullptr) && (m_playCount > 0); }

void AnimationPlayer::Update(const float& DeltaTimeIn)
{
	if (IsPlaying())
	{
		m_playTime += DeltaTimeIn * m_animationAssetCached->GetTicksPerSecond();

		const float Duration = m_animationAssetCached->GetDuration();
		const float Quotient = floorf(m_playTime / Duration);
		const float Remainder = m_playTime / Duration - Quotient;

		if (Quotient > 0)
		{
			m_playTime = m_playTime - Quotient * Duration;
			m_playCount--;
		}

		if (m_playCount > 0) // 플레이 중일 경우
		{

		}
		else // 플레이가 끝난 경우
		{
			StopAnimation();
		}
	}

	if (m_boneAssetCached)
	{
		const Bone* RootBone = m_boneAssetCached->GetRootBone();

		UpdateBoneFromParent(nullptr, RootBone);

		const vector<Bone*>& bones = m_boneAssetCached->GetBones();

		for (const Bone* bone : bones)
		{
			const size_t& BoneIndex = bone->GetBoneIndex();
			m_animationTransformation[BoneIndex] = XMMatrixTranspose(bone->GetOffsetMatrix() * m_animationTransformation[BoneIndex]);
		}
	}
}

void AnimationPlayer::UpdateBoneFromParent(const Bone* parentBone, const Bone* childBone)
{
	if (childBone != nullptr)
	{
		XMMATRIX childAnimationTransformation = XMMatrixIdentity();
		if (m_animationAssetCached != nullptr)
		{
			const AnimChannel* animChannel = m_animationAssetCached->GetAnimChannel(childBone->GetBoneName());
			if (animChannel != nullptr)
			{
				childAnimationTransformation = animChannel->GetLocalTransformation(m_playTime);
			}
		}
	
		if (parentBone != nullptr)
		{
			m_animationTransformation[childBone->GetBoneIndex()] = (childAnimationTransformation * m_animationTransformation[parentBone->GetBoneIndex()]);
		}
		else
		{
			m_animationTransformation[childBone->GetBoneIndex()] = childAnimationTransformation;
		}

		const list<Bone*>& GrandChildren = childBone->GetBoneChildren();
		for (Bone* GrandChild : GrandChildren)
		{
			UpdateBoneFromParent(childBone, GrandChild);
		}
	}
}
