#include "AnimationRetargeter.h"

#include "BoneAsset.h"
#include "AnimationAsset.h"

#include <format>
#include <set>

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
	else if (SourceBoneAsset == nullptr || DestBoneAsset == nullptr)
	{
		RetargetedFailedEvent.Invoke("Bone Profile Are Not Properly Set");
	}
	else if (SourceAnimationAsset == nullptr)
	{
		RetargetedFailedEvent.Invoke("Animation Profile Are Not Properly Set");
	}
	else
	{
		const float& Duration = SourceAnimationAsset->GetDuration();
		const float& TicksPerSecond = SourceAnimationAsset->GetTicksPerSecond();
		
		const unordered_map<string, AnimationChannel>& BoneNameToAnimChannels = SourceAnimationAsset->GetBoneNameToAnimationChannels();

		std::shared_ptr<AnimationAsset> RetargetedAnimation = make_shared<AnimationAsset>(AssetNameIn, false, Duration, TicksPerSecond);

		const map<string, XMMATRIX> TPoseLocalSourceTransformations = GetTPoseLocalTransformations(SourceBoneAsset);
		const map<string, XMMATRIX> TPoseLocalDestTransformations = GetTPoseLocalTransformations(DestBoneAsset);

		for (const auto& BoneTargeting : BoneTargetings)
		{
			const string& DestBoneName = BoneTargeting.first;
			const string& SourceBoneName = BoneTargeting.second;

			AnimationChannel NewChannel;

			if (BoneNameToAnimChannels.find(SourceBoneName) != BoneNameToAnimChannels.end())
			{
				const AnimationChannel& SourceBoneChannel = BoneNameToAnimChannels.at(SourceBoneName);

				set<float> TimeTable = SourceBoneChannel.GetTimeTable();
				for (const float& Time : TimeTable)
				{
					XMMATRIX SourceLocalAnimation = SourceAnimationAsset->GetAnimationLocalTransformation(SourceBoneName, Time);
					const XMMATRIX InvSourceTPose = XMMatrixInverse(nullptr, TPoseLocalSourceTransformations.at(SourceBoneName));
					const XMMATRIX DestTPose = TPoseLocalDestTransformations.at(DestBoneName);

					XMVECTOR Position;
					XMVECTOR Quaternion;
					XMVECTOR Scale;

					XMMatrixDecompose(&Scale, &Quaternion, &Position, SourceLocalAnimation * InvSourceTPose * DestTPose);

					NewChannel.AddPositionKey(Time, Position);
					NewChannel.AddQuaternionKey(Time, Quaternion);
					NewChannel.AddScaleKey(Time, Scale);
				}
			}
			RetargetedAnimation->AddAnimationChannel(DestBoneName, NewChannel);
		}

		return RetargetedAnimation;
	}
	return nullptr;
}

void AnimationRetargeter::GenerateBoneTargetings()
{
	BoneTargetings.clear();

	if (SourceBoneAsset != nullptr && DestBoneAsset != nullptr)
	{
		const map<string, Bone>& NameToFromBones = SourceBoneAsset->GetNameToBones();
		const map<string, Bone>& NameToToBones = DestBoneAsset->GetNameToBones();

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
}

void AnimationRetargeter::ReplaceTargetedSourceBone(const std::string& DestBoneName, const std::string NewSourceBoneName)
{
	if (BoneTargetings.find(DestBoneName) != BoneTargetings.end())
	{
		BoneTargetings[DestBoneName] = NewSourceBoneName;
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

map<string, XMMATRIX> AnimationRetargeter::GetTPoseLocalTransformations(const shared_ptr<BoneAsset>& BoneAssetIn)
{
	map<string, XMMATRIX> TPoseLocalTransformation;
	
	Bone* RootBone = BoneAssetIn->GetRootBone();
	const map<string, Bone>& NameToBones = BoneAssetIn->GetNameToBones();

	for (auto& NameToBone : NameToBones)
	{
		TPoseLocalTransformation[NameToBone.first] = XMMatrixIdentity();
	}

	Bone* CurrentBone = RootBone;
	UpdateTPoseLocalTransformation(TPoseLocalTransformation, CurrentBone);

	return TPoseLocalTransformation;
}

void AnimationRetargeter::UpdateTPoseLocalTransformation(map<string, XMMATRIX>& Transformations, Bone* CurrentBoneIn)
{
	if (CurrentBoneIn != nullptr)
	{
		const string& BoneName = CurrentBoneIn->GetBoneName();
		XMMATRIX Result = XMMatrixIdentity();

		Bone* ParentBone = CurrentBoneIn->GetParentBone();
		while (ParentBone != nullptr)
		{
			Result = XMMatrixMultiply(GetTPoseParentFactor(Transformations, ParentBone), Result);
			ParentBone = ParentBone->GetParentBone();
		}

		Transformations[BoneName] = XMMatrixMultiply(XMMatrixInverse(nullptr, CurrentBoneIn->GetOffsetMatrix()), Result);


		const vector<Bone*> ChildrenBones = CurrentBoneIn->GetChildrenBones();
		for (Bone* ChildBone : ChildrenBones)
		{
			UpdateTPoseLocalTransformation(Transformations, ChildBone);
		}
	}
}

XMMATRIX AnimationRetargeter::GetTPoseParentFactor(map<string, XMMATRIX>& Transformations, Bone* BoneIn)
{
	XMMATRIX BoneOffset = BoneIn->GetOffsetMatrix();
	XMMATRIX Result = XMMatrixIdentity();

	Bone* ParentBone = BoneIn->GetParentBone();
	while (ParentBone != nullptr)
	{
		const string& ParentName = ParentBone->GetBoneName();
		if (Transformations.find(ParentName) != Transformations.end())
		{
			Result = XMMatrixMultiply(Result, Transformations.at(ParentName));
		}

		ParentBone = ParentBone->GetParentBone();
	}
	Result = XMMatrixMultiply(Result, BoneOffset);

	return Result;
}
