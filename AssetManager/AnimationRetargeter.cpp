
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
		const float& duration = m_sourceAnimationAsset->GetDuration();
		const float& ticksPerSecond = m_sourceAnimationAsset->GetTicksPerSecond();

		const unordered_map<string, AnimChannel>& sourceBoneNameToAnimChannels = m_sourceAnimationAsset->GetBoneNameToAnimChannels();
		AnimationAsset* retargetedAnimation = new AnimationAsset(assetName);
		retargetedAnimation->SetAnimationDuration(duration, ticksPerSecond);

		const map<string, XMMATRIX> localSourceTPoseTransformations = GetLocalTPoseTransformations(m_sourceBoneAsset);
		const map<string, XMMATRIX> localTargetTPoseTransformations = GetLocalTPoseTransformations(m_targetBoneAsset);

		for (const auto& boneTargeting : m_boneTargetings)
		{
			const Bone* const sourceBone = boneTargeting.first;
			const Bone* const targetingBone = boneTargeting.second;

			const string& sourceBoneName = sourceBone ? sourceBone->GetBoneName() : "";
			const string& targetBoneName = targetingBone ? targetingBone->GetBoneName() : "";

			XMVECTOR position;
			XMVECTOR quaternion;
			XMVECTOR scale;

			if (sourceBoneNameToAnimChannels.find(targetBoneName) != sourceBoneNameToAnimChannels.end())
			{
				AnimChannel newChannel;
				const AnimChannel& sourceBoneChannel = sourceBoneNameToAnimChannels.at(targetBoneName);

				const set<float>& TimeTable = sourceBoneChannel.GetTimeTable();
				for (const float& time : TimeTable)
				{
					XMMATRIX sourceLocalAnimation = sourceBoneChannel.GetLocalTransformation(time);
					const XMMATRIX invSourceTPose = XMMatrixInverse(nullptr, localSourceTPoseTransformations.at(targetBoneName));
					const XMMATRIX targetTPose = localTargetTPoseTransformations.at(targetBoneName);


					XMMatrixDecompose(&scale, &quaternion, &position, sourceLocalAnimation * invSourceTPose * targetTPose);

					newChannel.AddPositionKey(time, position);
					newChannel.AddQuaternionKey(time, quaternion);
					newChannel.AddScaleKey(time, scale);
				}
				retargetedAnimation->AddAnimChannel(targetBoneName, move(newChannel));
			}
			else
			{
				const XMMATRIX targetTPose = localTargetTPoseTransformations.at(targetBoneName);
				XMMatrixDecompose(&scale, &quaternion, &position, targetTPose);

				AnimChannel identityChannel;
				identityChannel.AddPositionKey(0.f, position);
				identityChannel.AddPositionKey(duration, position);
				identityChannel.AddQuaternionKey(0.f, quaternion);
				identityChannel.AddQuaternionKey(duration, quaternion);
				identityChannel.AddScaleKey(0.f, scale);
				identityChannel.AddScaleKey(duration, scale);

				retargetedAnimation->AddAnimChannel(targetBoneName, move(identityChannel));
			}
		}
		return retargetedAnimation;
	}
	return nullptr;
}

void AnimationRetargeter::GenerateBoneTargetings()
{
	m_boneTargetings.clear();

	if (m_sourceBoneAsset != nullptr && m_targetBoneAsset != nullptr)
	{
		const vector<Bone*>& sourceBones = m_sourceBoneAsset->GetBones();
		const vector<Bone*>& targetBones = m_targetBoneAsset->GetBones();

		bool isSourceMain = sourceBones.size() > targetBones.size();
		const vector<Bone*>& mainBones = isSourceMain ? sourceBones : targetBones;
		const vector<Bone*>& subBones = isSourceMain ? targetBones : sourceBones;

		unordered_map<string, Bone*> subBoneNameToBones;
		for (auto& subBone : subBones)
		{
			subBoneNameToBones.emplace(subBone->GetBoneName(), subBone);
		}

		for (auto& mainBone : mainBones)
		{
			const string& mainBoneName = mainBone->GetBoneName();
			if (subBoneNameToBones.find(mainBoneName) != subBoneNameToBones.end())
			{
				m_boneTargetings.emplace_back(make_pair(isSourceMain ? mainBone : subBoneNameToBones[mainBoneName], isSourceMain ? subBoneNameToBones[mainBoneName] : mainBone));
			}
			else
			{
				m_boneTargetings.emplace_back(make_pair(isSourceMain ? mainBone : nullptr, isSourceMain ? nullptr : mainBone));
			}
		}
	}
}

void AnimationRetargeter::ReplaceTargetBone(const size_t& idx, const Bone* const sourceBone, const Bone* const targetBone)
{
	if (m_boneTargetings.size() > idx)
	{
		m_boneTargetings[idx] = make_pair(sourceBone, targetBone);
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
