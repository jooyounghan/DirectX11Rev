#pragma once
#include <memory>
#include <map>
#include <unordered_map>
#include <string>

namespace DirectX
{
	struct XMMATRIX;
}
class Bone;
class BoneAsset;
class AnimationAsset;

class AnimationRetargeter
{
public:
	AnimationRetargeter();
	~AnimationRetargeter();

protected:
	const BoneAsset* m_sourceBoneAsset = nullptr;
	const BoneAsset* m_targetBoneAsset = nullptr;

protected:
	const AnimationAsset* m_sourceAnimationAsset = nullptr;

protected:
	std::unordered_map<const Bone*, const Bone*> m_boneTargetings;

public:
	inline void SetSourceBoneAsset(const BoneAsset* sourceBoneAsset) { m_sourceBoneAsset = sourceBoneAsset; }
	inline void SetTargetBoneAsset(const BoneAsset* targetBoneAsset) { m_targetBoneAsset = targetBoneAsset; }
	inline void SetSourceAnimationAsset(const AnimationAsset* sourceAnimationAsset) { m_sourceAnimationAsset = sourceAnimationAsset; }
	inline const BoneAsset* GetSourceBoneAsset() { return m_sourceBoneAsset; }
	inline const BoneAsset* GetTargetBoneAsset() { return m_targetBoneAsset; }
	inline const AnimationAsset* GetSourceAnimationAsset() { return m_sourceAnimationAsset; }

public:
	const std::unordered_map<const Bone*, const Bone*>& GetBoneTargetings() { return m_boneTargetings; }

public:
	inline bool IsAvailableRetargeting() { return m_sourceAnimationAsset && m_targetBoneAsset && m_sourceAnimationAsset; }
	AnimationAsset* GetRetargetedAnimation(const std::string& assetName);

public:
	void GenerateBoneTargetings();
	void ReplaceTargetBone(const Bone* const sourceBone, const Bone* const targetBone);

private:
	bool IsSameProfile(
		const BoneAsset* const boneAssetIn,
		const AnimationAsset* const animationAssetIn
	);

private:
	std::map<std::string, DirectX::XMMATRIX> GetLocalTPoseTransformations(const BoneAsset* const boneAssetIn);
};

