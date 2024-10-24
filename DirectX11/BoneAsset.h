#pragma once
#include "AAssetFile.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include "directxmath/DirectXMath.h"

constexpr const char* BoneAssetOutPath = ".\\Assets\\Bone\\";

class Bone : public IOnSerializableMap
{
	friend class BoneAsset;

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
	std::string BoneName;
	MakeGetter(BoneName);

private:
	DirectX::XMMATRIX OffsetMatrix;
	MakeGetter(OffsetMatrix);

private:
	std::vector<Bone*> ChildrenBones;
	MakeGetter(ChildrenBones);

public:
	void AddChildBone(Bone* ChildBone);

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

class BoneAsset : public AAssetFile
{
public:
	BoneAsset(const std::string& AssetNameIn, bool LoadFromAsset);
	virtual ~BoneAsset();

public:
	static std::string BoneAssetKind;

protected:
	Bone* RootBone = nullptr;
	MakeSetterGetter(RootBone);

protected:
	Bone* CurrentBone = nullptr;
	MakeSetterGetter(CurrentBone);

protected:
	std::unordered_map<std::string, Bone> NameToBones;

public:
	Bone* FindBone(const std::string& BoneName);
	void AddBone(const std::string& BoneName, const size_t& BoneIdxIn, DirectX::XMMATRIX BoneOffsetMatrix);

public:
	void TraverseDownBone(Bone* NewBone);
	void TraverseUpBone();

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

