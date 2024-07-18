#pragma once
#include "IAssetFile.h"

#include <vector>
#include <map>
#include <memory>
#include <DirectXMath.h>

class Bone
{
public:
	Bone();
	Bone(const std::string& NameIn, const size_t& BoneIdxIn, const DirectX::XMMATRIX& OffsetMatrixIn);
	~Bone();

private:
	size_t BoneIdx = NULL;

private:
	Bone* ParentBone = nullptr;
	MakeSetterGetter(ParentBone);

private:
	std::vector<Bone*> ChildrenBones;
	MakeGetter(ChildrenBones);

private:
	std::string BoneName;
	MakeGetter(BoneName);

private:
	DirectX::XMMATRIX OffsetMatrix;
	MakeGetter(OffsetMatrix);

public:
	void AddChildBone(Bone* ChildBone)
	{
		ChildBone->SetParentBone(this);
		ChildrenBones.push_back(ChildBone);
	}
};

class BoneAsset : public IAssetFile
{
public:
	BoneAsset(const std::string& AssetNameIn);
	virtual ~BoneAsset();

protected:
	Bone* RootBone = nullptr;
	MakeSetterGetter(RootBone);

protected:
	Bone* CurrentBone = nullptr;
	MakeSetterGetter(CurrentBone);

protected:
	std::map<std::string, Bone> NameToBones;

public:
	Bone* FindBone(const std::string& BoneName);
	void AddBone(const std::string& BoneName, const size_t& BoneIdxIn, DirectX::XMMATRIX BoneOffsetMatrix);

public:
	void TraverseDownBone(Bone* NewBone);
	void TraverseUpBone();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath) override;
	virtual void Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn) override;
};

