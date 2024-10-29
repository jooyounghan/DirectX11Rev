#include "AnimationPlayer.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"

#include "GlobalVariable.h"
#include "UploadableBufferManager.h"
#include "StructuredBuffer.h"

using namespace std;
using namespace DirectX;

AnimationPlayer::AnimationPlayer()
{
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::SetBoneAsset(const std::shared_ptr<BoneAsset>& BoneAssetIn)
{
	StopAnimation(false);

	if (AnimationTransformationBuffer != nullptr)
	{
		AnimationTransformationBuffer->BeginRemoveEvent.Invoke(AnimationTransformationBuffer.get());
		AnimationTransformationBuffer = nullptr;
	}

	BoneAssetCached = BoneAssetIn;
	AnimationTransformationBuffer = App::GUploadableBufferManager->CreateUploadableBuffer<StructuredBuffer<XMMATRIX>>(BoneAssetCached->GetNameToBones().size());
}

void AnimationPlayer::PlayAnimation(const shared_ptr<AnimationAsset>& AnimationAssetIn, const size_t& PlayCountIn)
{
	if (BoneAssetCached != nullptr)
	{
		AnimationAssetCached = AnimationAssetIn;
		PlayCount = PlayCountIn;
		PlayTime = 0.f;
	}
}

void AnimationPlayer::StopAnimation(const bool& IsRaiseEvent)
{
	AnimationAssetCached = nullptr;
	PlayCount = 0;
	PlayTime = 0.f;

	if (IsRaiseEvent) AnimationEndedEvent.Invoke();
}

inline bool AnimationPlayer::IsPlaying() { return (AnimationAssetCached != nullptr) && (PlayCount > 0); }

void AnimationPlayer::Update(const float& DeltaTimeIn)
{
	if (IsPlaying())
	{
		PlayTime += DeltaTimeIn * AnimationAssetCached->GetTicksPerSecond();

		const float Duration = AnimationAssetCached->GetDuration();
		const float Quotient = floorf(PlayTime / Duration);
		const float Remainder = PlayTime / Duration - Quotient;

		if (Quotient > 0)
		{
			PlayTime = PlayTime - Quotient * Duration;
			PlayCount--;
		}

		if (PlayCount > 0) // 플레이 중일 경우
		{

		}
		else // 플레이가 끝난 경우
		{
			StopAnimation();
		}

		unordered_map<string, Bone> NameToBones = BoneAssetCached->GetNameToBones();
		for (const auto& NameToBone : NameToBones)
		{
			const string& BoneName = NameToBone.first;
			const Bone& BoneData = NameToBone.second;

			const XMMATRIX AnimationTransformation =
				AnimationAssetCached != nullptr ? 
				AnimationAssetCached->GetAnimationTransformation(BoneName, PlayTime) * BoneData.GetOffsetMatrix() :
				XMMatrixIdentity() * BoneData.GetOffsetMatrix();

			AnimationTransformationBuffer->SetStagedData(BoneData.GetBoneIndex(), AnimationTransformation);
		}

	}
}
