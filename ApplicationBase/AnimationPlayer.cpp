#include "AnimationPlayer.h"

#include "BoneAsset.h"
#include "AnimationAsset.h"

#include "StructuredBuffer.h"

using namespace std;
using namespace DirectX;


AnimationPlayer::AnimationPlayer(const BoneAsset* boneAsset)
	: m_boneAssetCached(boneAsset)
{

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

void AnimationPlayer::StopAnimation()
{
	m_animationAssetCached = nullptr;
	m_playCount = 0;
	m_playTime = 0.f;
	m_animationEndedEvent();
}

inline bool AnimationPlayer::IsPlaying() { return (m_animationAssetCached != nullptr) && (m_playCount > 0); }


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
			m_boneTransformation[childBone->GetBoneIndex()] =
				childAnimationTransformation * m_boneTransformation[parentBone->GetBoneIndex()];
		}
		else
		{
			m_boneTransformation[childBone->GetBoneIndex()] = childAnimationTransformation;
		}

		const list<Bone*>& GrandChildren = childBone->GetBoneChildren();
		for (Bone* GrandChild : GrandChildren)
		{
			UpdateBoneFromParent(childBone, GrandChild);
		}
	}
}

void AnimationPlayer::InitAnimationPlayer(ID3D11Device* const device)
{
	m_boneTransformation.resize(m_boneAssetCached->GetBones().size(), XMMatrixIdentity());

	m_boneTransformationBuffer = new StructuredBuffer(sizeof(XMMATRIX), static_cast<UINT>(m_boneTransformation.size()));
	m_boneTransformationBuffer->Initialize(device);
}

void AnimationPlayer::UpdateAnimationPlayer(ID3D11DeviceContext* const deviceContext, const float& deltaTime)
{
	if (IsPlaying())
	{
		m_playTime += deltaTime * m_animationAssetCached->GetTicksPerSecond();

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
			m_boneTransformation[BoneIndex] = XMMatrixTranspose(bone->GetOffsetMatrix() * m_boneTransformation[BoneIndex]);
		}
		
		m_boneTransformationBuffer->Upload(
			deviceContext, sizeof(XMMATRIX), 
			static_cast<UINT>(m_boneTransformation.size()), 
			m_boneTransformation.data()
		);
	}
}
