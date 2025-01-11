#pragma once
#include "Asset.h"
#include <DirectXMath.h>

#include <map>
#include <list>
#include <memory>

class Bone : public ISerializable
{
public:
	Bone() = default;
	~Bone() override;

private:
	std::string m_boneName;
	uint32_t m_boneIdx = NULL;
	DirectX::XMMATRIX m_offsetMatrix = DirectX::XMMatrixIdentity();
	Bone* m_parentBone = nullptr;
	std::list<Bone*> m_boneChildren;

public:
	inline const std::string& GetBoneName() const { return m_boneName; }
	inline const uint32_t& GetBoneIndex() const { return m_boneIdx; }
	inline const DirectX::XMMATRIX GetOffsetMatrix() const { return m_offsetMatrix; }
	inline const Bone* GetParentBone() const { return m_parentBone; }
	inline const std::list<Bone*>& GetBoneChildren() const { return m_boneChildren; }

public:
	void SetBoneProperties(
		const std::string& boneName, const uint32_t& boneIdxIn, const DirectX::XMMATRIX offsetMatrix
	);
	void SetParentBone(Bone* const parentBone);
	void AddChildBone(Bone* const childBone);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class BoneAsset : public AAsset
{
public:
	BoneAsset() = default;
	BoneAsset(const std::string& assetName);
	~BoneAsset() override;

protected:
	Bone* m_rootBone = nullptr;
	std::vector<Bone*> m_bones;

public:
	inline const Bone* GetRootBone() const { return m_rootBone; }
	inline const std::vector<Bone*>& GetBones() const { return m_bones; }

public:
	inline void SetRootBone(Bone* const bone) { m_rootBone = bone; }
	inline void AddBone(Bone* const bone) { m_bones.emplace_back(bone); }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;
};

class IBoneProvider
{
public:
	virtual BoneAsset* const GetBoneAsset(
		const std::string& textureName
	) = 0;
};

