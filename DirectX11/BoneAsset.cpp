#include "BoneAsset.h"

using namespace DirectX;

Bone::Bone()
	: BoneName(""), BoneIdx(NULL), OffsetMatrix(XMMatrixIdentity())
{
}

Bone::Bone(
	const std::string& NameIn, 
	const size_t& BoneIdxIn, 
	const DirectX::XMMATRIX& OffsetMatrixIn
)
	: BoneName(NameIn), BoneIdx(BoneIdxIn), OffsetMatrix(OffsetMatrixIn)
{
}

Bone::~Bone()
{
}

BoneAsset::BoneAsset(const std::string& AssetNameIn)
	: IAssetFile(AssetNameIn + "_Bone", EAssetType::Bone)
{
}


BoneAsset::~BoneAsset()
{
}

Bone* BoneAsset::FindBone(const std::string& BoneName)
{
	Bone* Result = nullptr;
	if (NameToBones.find(BoneName) != NameToBones.end())
	{
		Result = &NameToBones[BoneName];
	}
	return Result;
}

void BoneAsset::AddBone(const std::string& BoneName, const size_t& BoneIdxIn, DirectX::XMMATRIX BoneOffsetMatrix)
{
	if (NameToBones.find(BoneName) == NameToBones.end())
	{
		NameToBones.emplace(BoneName, Bone(BoneName, BoneIdxIn, BoneOffsetMatrix));
	}
}

void BoneAsset::TraverseDownBone(Bone* NewBone)
{
	if (CurrentBone == nullptr)
	{
		RootBone = NewBone;
		CurrentBone = NewBone;
	}
	else
	{
		CurrentBone->AddChildBone(NewBone);
		CurrentBone = NewBone;
	}
}

void BoneAsset::TraverseUpBone()
{
	if (CurrentBone != nullptr)
	{
		CurrentBone = CurrentBone->GetParentBone();
	}
}

void BoneAsset::Serialize(const std::string& OutputAdditionalPath)
{
}

void BoneAsset::Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn)
{
}

