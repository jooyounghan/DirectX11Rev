
#include "AnimationRetargeter.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"

#include <unordered_set>

using namespace std;
using namespace DirectX;

AnimationRetargeter::AnimationRetargeter()
{
}

AnimationRetargeter::~AnimationRetargeter()
{
}

AnimationAsset* AnimationRetargeter::GetRetargetedAnimation(const std::string& assetName)
{
	if (m_boneTargetings.empty())
	{
		return nullptr;
	}
	else
	{
		const float& Duration = m_sourceAnimationAsset->GetDuration();
		const float& TicksPerSecond = m_sourceAnimationAsset->GetTicksPerSecond();

		const unordered_map<string, AnimChannel>& sourceBoneNameToAnimChannels = m_sourceAnimationAsset->GetBoneNameToAnimChannels();
		AnimationAsset* RetargetedAnimation = new AnimationAsset(assetName);
		RetargetedAnimation->SetAnimationDuration(Duration, TicksPerSecond);

		const map<string, XMMATRIX> localSourceTPoseTransformations = GetLocalTPoseTransformations(m_sourceBoneAsset);
		const map<string, XMMATRIX> localTargetTPoseTransformations = GetLocalTPoseTransformations(m_targetBoneAsset);

		for (const auto& boneTargeting : m_boneTargetings)
		{
			const Bone* const sourceBone = boneTargeting.first;
			const Bone* const targetingBone = boneTargeting.second;

			const string& sourceBoneName = sourceBone ? sourceBone->GetBoneName() : "";
			const string& targetBoneName = targetingBone ? targetingBone->GetBoneName() : "";

			XMVECTOR Position;
			XMVECTOR Quaternion;
			XMVECTOR Scale;

			if (sourceBoneNameToAnimChannels.find(targetBoneName) != sourceBoneNameToAnimChannels.end())
			{
				AnimChannel newChannel;
				const AnimChannel& SourceBoneChannel = sourceBoneNameToAnimChannels.at(targetBoneName);

				const set<float>& TimeTable = SourceBoneChannel.GetTimeTable();
				for (const float& Time : TimeTable)
				{
					XMMATRIX SourceLocalAnimation = SourceBoneChannel.GetLocalTransformation(Time);
					const XMMATRIX InvSourceTPose = XMMatrixInverse(nullptr, localSourceTPoseTransformations.at(targetBoneName));
					const XMMATRIX DestTPose = localTargetTPoseTransformations.at(sourceBoneName);


					XMMatrixDecompose(&Scale, &Quaternion, &Position, SourceLocalAnimation * InvSourceTPose * DestTPose);

					newChannel.AddPositionKey(Time, Position);
					newChannel.AddQuaternionKey(Time, Quaternion);
					newChannel.AddScaleKey(Time, Scale);
				}
				RetargetedAnimation->AddAnimChannel(targetBoneName, move(newChannel));
			}
			else
			{
				const XMMATRIX DestTPose = localTargetTPoseTransformations.at(sourceBoneName);
				XMMatrixDecompose(&Scale, &Quaternion, &Position, DestTPose);

				AnimChannel identityChannel;
				identityChannel.AddPositionKey(0.f, Position);
				identityChannel.AddPositionKey(Duration, Position);
				identityChannel.AddQuaternionKey(0.f, Quaternion);
				identityChannel.AddQuaternionKey(Duration, Quaternion);
				identityChannel.AddScaleKey(0.f, Scale);
				identityChannel.AddScaleKey(Duration, Scale);

				RetargetedAnimation->AddAnimChannel(targetBoneName, move(identityChannel));
			}
		}
		return RetargetedAnimation;
	}
	return nullptr;
}

void AnimationRetargeter::GenerateBoneTargetings()
{
	m_boneTargetings.clear();

	if (m_sourceBoneAsset != nullptr && m_targetBoneAsset != nullptr)
	{
		const vector<Bone*>& sourceBones = m_sourceBoneAsset->GetBones();
		const vector<Bone*>& destBones = m_targetBoneAsset->GetBones();

		unordered_map<string, Bone*> sourceNameToBones;
		for (auto& sourceBone : sourceBones)
		{
			sourceNameToBones.emplace(sourceBone->GetBoneName(), sourceBone);
		}

		for (auto& destBone : destBones)
		{
			const string& destBoneName = destBone->GetBoneName();
			if (sourceNameToBones.find(destBoneName) != sourceNameToBones.end())
			{
				m_boneTargetings.emplace(destBone, sourceNameToBones[destBoneName]);
			}
			else
			{
				m_boneTargetings.emplace(destBone, nullptr);
			}
		}
	}
}

void AnimationRetargeter::ReplaceTargetBone(const Bone* const sourceBone, const Bone* const targetBone)
{
	if (m_boneTargetings.find(sourceBone) != m_boneTargetings.end())
	{
		m_boneTargetings[sourceBone] = targetBone;
	}
}

bool AnimationRetargeter::IsSameProfile(const BoneAsset* const boneAssetIn, const AnimationAsset* const animationAssetIn)
{
	if (boneAssetIn == nullptr)
	{
		return false;
	}
	if (animationAssetIn == nullptr)
	{
		return false;
	}

	const vector<Bone*>& bones = boneAssetIn->GetBones();
	const unordered_map<string, AnimChannel>& boneNameToAnimationChannels = animationAssetIn->GetBoneNameToAnimChannels();

	for (const Bone* bone : bones)
	{
		if (boneNameToAnimationChannels.find(bone->GetBoneName()) == boneNameToAnimationChannels.end())
		{
			return false;
		}
	}

	return true;
}

map<string, XMMATRIX> AnimationRetargeter::GetLocalTPoseTransformations(const BoneAsset* const boneAssetIn)
{
	map<string, XMMATRIX> tPoseLocalTransformation;

	const Bone* rootBone = boneAssetIn->GetRootBone();
	const vector<Bone*>& bones = boneAssetIn->GetBones();

	for (auto& bone : bones)
	{
		tPoseLocalTransformation[bone->GetBoneName()] = XMMatrixIdentity();
	}

	function<void(const Bone* const)> updateTLocalTransformation = [&](const Bone* const currentBone)
		{
			if (currentBone != nullptr)
			{
				const string& boneName = currentBone->GetBoneName();
				const Bone* parentBone = currentBone->GetParentBone();

				tPoseLocalTransformation[boneName] 
					= XMMatrixMultiply(
						XMMatrixInverse(nullptr, currentBone->GetOffsetMatrix()),
						parentBone != nullptr ? parentBone->GetOffsetMatrix() : XMMatrixIdentity()
					);

				const list<Bone*> childrenBone = currentBone->GetBoneChildren();
				for (Bone* const childBone : childrenBone)
				{
					updateTLocalTransformation(childBone);
				}
			}
		};

	updateTLocalTransformation(rootBone);

	return tPoseLocalTransformation;

}
