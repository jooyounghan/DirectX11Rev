#include "AnimationRetargeter.h"

#include "BoneAsset.h"
#include "AnimationAsset.h"

#include <format>

using namespace std;
using namespace DirectX;

AnimationRetargeter::AnimationRetargeter()
{
}

AnimationRetargeter::~AnimationRetargeter()
{
}

shared_ptr<AnimationAsset> AnimationRetargeter::GetRetargetedAnimation(const string& AssetNameIn)
{
	if (BoneTargetings.empty())
	{
		RetargetedFailedEvent.Invoke("Bone Targeting Is Not Genereated");
	}
	else if (FromBoneAsset == nullptr || ToBoneAsset == nullptr || TargetAnimationAsset == nullptr)
	{
		RetargetedFailedEvent.Invoke("Bone And Animation Profile Are Not Properly Set");

	}
	else
	{
		const float& Duration = TargetAnimationAsset->GetDuration();
		const float& TicksPerSecond = TargetAnimationAsset->GetTicksPerSecond();

		std::shared_ptr<AnimationAsset> RetargetedAnimation = make_shared<AnimationAsset>(AssetNameIn, false, Duration, TicksPerSecond);
		
		const unordered_map<string, AnimationChannel>& FromBoneNameToAnimationChannels = TargetAnimationAsset->GetBoneNameToAnimationChannels();
		const map<string, Bone>& NameToFromBones = FromBoneAsset->GetNameToBones();
		const map<string, Bone>& NameToToBones = ToBoneAsset->GetNameToBones();

		AnimationChannel IdentityChannel;
		IdentityChannel.AddPositionKey(0.f, XMVectorZero());
		IdentityChannel.AddPositionKey(Duration, XMVectorZero());
		IdentityChannel.AddQuaternionKey(0.f, XMQuaternionIdentity());
		IdentityChannel.AddQuaternionKey(Duration, XMQuaternionIdentity());
		IdentityChannel.AddScaleKey(0.f, XMVectorSet(1.f, 1.f, 1.f, 0.f));
		IdentityChannel.AddScaleKey(Duration, XMVectorSet(1.f, 1.f, 1.f, 0.f));

		for (auto& BoneTargeting : BoneTargetings)
		{
			const string& ToBoneName = BoneTargeting.first;
			const string& FromBoneName = BoneTargeting.second;

			if (FromBoneNameToAnimationChannels.find(FromBoneName) != FromBoneNameToAnimationChannels.end())
			{
				const AnimationChannel& TargetAnimationChannel = FromBoneNameToAnimationChannels.at(FromBoneName);
				const vector<AnimationKey>& TargetPositionKeys = TargetAnimationChannel.GetPositionKeys();
				const vector<AnimationKey>& TargetQuaternionKeys = TargetAnimationChannel.GetQuaternionKeys();
				const vector<AnimationKey>& TargetScaleKeys = TargetAnimationChannel.GetScaleKeys();

				XMMATRIX RetargetingMatrix = NameToFromBones.at(FromBoneName).GetOffsetMatrix() * XMMatrixInverse(nullptr, NameToToBones.at(ToBoneName).GetOffsetMatrix());
				
				XMVECTOR RetargetingPosition;
				XMVECTOR RetargetingQuaterion;
				XMVECTOR RetargetingScale;

				XMMatrixDecompose(&RetargetingScale, &RetargetingQuaterion, &RetargetingPosition, RetargetingMatrix);

				AnimationChannel RetargetedAnimationChannel;

				for (auto& TargetPositionKey : TargetPositionKeys)
				{
					RetargetedAnimationChannel.AddPositionKey(TargetPositionKey.GetTime(), TargetPositionKey.GetAnimationData() + RetargetingPosition);
				}
				for (auto& TargetQuaternionKey : TargetQuaternionKeys)
				{
					RetargetedAnimationChannel.AddQuaternionKey(TargetQuaternionKey.GetTime(), XMQuaternionDot(TargetQuaternionKey.GetAnimationData(), RetargetingQuaterion));
				}
				for (auto& TargetScaleKey : TargetScaleKeys)
				{
					RetargetedAnimationChannel.AddScaleKey(TargetScaleKey.GetTime(), XMVectorMultiply(TargetScaleKey.GetAnimationData(), RetargetingScale));
				}

				RetargetedAnimation->AddAnimationChannel(ToBoneName, RetargetedAnimationChannel);
			}
			else
			{
				RetargetedAnimation->AddAnimationChannel(ToBoneName, IdentityChannel);
			}

		}

		return RetargetedAnimation;
	}
	return nullptr;
}

void AnimationRetargeter::GenerateBoneTargetings()
{
	BoneTargetings.clear();

	if (FromBoneAsset != nullptr && ToBoneAsset != nullptr)
	{
		const map<string, Bone>& NameToFromBones = FromBoneAsset->GetNameToBones();
		const map<string, Bone>& NameToToBones = ToBoneAsset->GetNameToBones();

		for (auto& NameToToBone : NameToToBones)
		{
			if (NameToFromBones.find(NameToToBone.first) != NameToFromBones.end())
			{
				BoneTargetings.emplace(NameToToBone.first, NameToToBone.first);
			}
			else
			{
				BoneTargetings.emplace(NameToToBone.first, "");
			}
		}
	}


	//const unordered_map<string, AnimationChannel>& BoneNameToAnimationChannels = TargetAnimationAssetIn->GetBoneNameToAnimationChannels();

	//const float& Duration = TargetAnimationAsset->GetDuration();
	//const float& TicksPerSecond = TargetAnimationAsset->GetTicksPerSecond();

	//RetrargetedBoneNameToAnimationChannels.clear();



	//for (auto& NameToBone : NameToBones)
	//{
	//	const string& BoneName = NameToBone.first;
	//	if (BoneNameToAnimationChannels.find(BoneName) != BoneNameToAnimationChannels.end())
	//	{
	//		RetrargetedBoneNameToAnimationChannels.emplace(NameToBone.first, BoneNameToAnimationChannels.at(BoneName));
	//	}
	//	else
	//	{
	//		RetrargetedBoneNameToAnimationChannels.emplace(NameToBone.first, IdentityChannel);
	//	}
	//}
}

void AnimationRetargeter::ReplaceTargetedFromBone(const std::string& ToBoneName, const std::string NewFromBoneName)
{
	if (BoneTargetings.find(ToBoneName) != BoneTargetings.end())
	{
		BoneTargetings[ToBoneName] = NewFromBoneName;
	}
}

bool AnimationRetargeter::IsSameProfile(
	const std::shared_ptr<BoneAsset>& BoneAssetIn, 
	const std::shared_ptr<AnimationAsset>& AnimationAssetIn
)
{
	if (BoneAssetIn != nullptr)
	{
		RetargetedFailedEvent.Invoke("Bone Asset Is Not Set");
		return false;
	}
	if (AnimationAssetIn != nullptr)
	{
		RetargetedFailedEvent.Invoke("Animation Asset Is Not Set");
		return false;
	}

	const map<string, Bone>& NameToBones = BoneAssetIn->GetNameToBones();
	const unordered_map<string, AnimationChannel>& BoneNameToAnimationChannels = AnimationAssetIn->GetBoneNameToAnimationChannels();

	for (const auto& NameToBone : NameToBones)
	{
		if (BoneNameToAnimationChannels.find(NameToBone.first) == BoneNameToAnimationChannels.end())
		{
			RetargetedFailedEvent.Invoke("Profiles Between Bone Asset And Animation Asset Are Not Same");
			return false;
		}
	}

	return true;
}
