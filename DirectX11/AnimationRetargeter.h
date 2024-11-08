#pragma once

#include "HeaderHelper.h"
#include "Delegation.h"

#include <string>
#include <memory>

class AssetManager;
class BoneAsset;
class AnimationAsset;
class AnimationChannel;

typedef std::function<void(const std::string&)> RetargetedFailedHandler;

class AnimationRetargeter
{
public:
	AnimationRetargeter();
	~AnimationRetargeter();

protected:
	std::shared_ptr<BoneAsset> FromBoneAsset;
	std::shared_ptr<BoneAsset> ToBoneAsset;
	MakeSmartPtrSetterGetter(FromBoneAsset);
	MakeSmartPtrSetterGetter(ToBoneAsset);

protected:
	std::shared_ptr<AnimationAsset> TargetAnimationAsset;
	MakeSmartPtrSetterGetter(TargetAnimationAsset);

protected:
	std::unordered_map<std::string, std::string> BoneTargetings;
	MakeGetter(BoneTargetings);

public:
	std::shared_ptr<AnimationAsset> GetRetargetedAnimation(const std::string& AssetNameIn);

public:
	void GenerateBoneTargetings();
	void ReplaceTargetedFromBone(const std::string& ToBoneName, const std::string FromBoneName);

private:
	bool IsSameProfile(
		const std::shared_ptr<BoneAsset>& BoneAssetIn,
		const std::shared_ptr<AnimationAsset>& AnimationAssetIn
	);

public:
	Delegation<const std::string&> RetargetedFailedEvent;
};

