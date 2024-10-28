#include "AnimationPlayer.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"

using namespace std;

AnimationPlayer::AnimationPlayer(const shared_ptr<BoneAsset>& BoneAssetIn)
	: BoneAssetCached(BoneAssetIn)
{
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::PlayAnimation(const shared_ptr<AnimationAsset>& AnimationAssetIn, const size_t& PlayCountIn)
{
	AnimationAssetCached = AnimationAssetIn;
	PlayCount = PlayCountIn;
	PlayTime = 0.f;
}

void AnimationPlayer::StopAnimation()
{
	AnimationAssetCached = nullptr;
	PlayCount = 0;
	PlayTime = 0.f;
}

inline bool AnimationPlayer::IsPlaying() { return (AnimationAssetCached != nullptr) && (PlayCount > 0); }

void AnimationPlayer::Update(const float& DeltaTimeIn)
{
	if (IsPlaying())
	{
		PlayTime += DeltaTimeIn * AnimationAssetCached->GetTicksPerSecond();

		if (PlayTime > AnimationAssetCached->GetDuration())
		{
			PlayCount--;
			PlayTime = 0;
			return;
		}

		unordered_map<string, Bone> NameToBones = BoneAssetCached->GetNameToBones();
		for (const auto& NameToBone : NameToBones)
		{
			const string& BoneName = NameToBone.first;
			const Bone& BoneData = NameToBone.second;

			AnimationAssetCached->GetAnimationTransformation(BoneName, PlayTime) * BoneData.GetOffsetMatrix();
		}

	}
}
