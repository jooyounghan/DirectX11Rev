#pragma once

#include "HeaderHelper.h"
#include "Delegation.h"
#include "directxmath/DirectXMath.h"

#include <string>
#include <map>
#include <memory>

class AssetManager;
class Bone;
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
	std::shared_ptr<BoneAsset> SourceBoneAsset;
	std::shared_ptr<BoneAsset> DestBoneAsset;
	MakeSmartPtrSetterGetter(SourceBoneAsset);
	MakeSmartPtrSetterGetter(DestBoneAsset);

protected:
	std::shared_ptr<AnimationAsset> SourceAnimationAsset;
	MakeSmartPtrSetterGetter(SourceAnimationAsset);

protected:
	std::unordered_map<std::string, std::string> BoneTargetings;
	MakeGetter(BoneTargetings);

public:
	std::shared_ptr<AnimationAsset> GetRetargetedAnimation(const std::string& AssetNameIn);

public:
	void GenerateBoneTargetings();
	void ReplaceTargetedSourceBone(const std::string& DesBoneName, const std::string SourceBoneName);

private:
	bool IsSameProfile(
		const std::shared_ptr<BoneAsset>& BoneAssetIn,
		const std::shared_ptr<AnimationAsset>& AnimationAssetIn
	);

private:
	std::map<std::string, DirectX::XMMATRIX> GetTPoseLocalTransformations(const std::shared_ptr<BoneAsset>& BoneAssetIn);
	void UpdateTPoseLocalTransformation(std::map<std::string, DirectX::XMMATRIX>& Transformations, Bone* CurrentBoneIn);
	DirectX::XMMATRIX GetTPoseParentFactor(std::map<std::string, DirectX::XMMATRIX>& Transformations, Bone* BoneIn);

public:
	Delegation<const std::string&> RetargetedFailedEvent;
};

