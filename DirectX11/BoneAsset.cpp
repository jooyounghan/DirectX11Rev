#include "BoneAsset.h"
#include <stdio.h>

using namespace std;
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
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);
		OnSerializing(RootBone, OutputAssetFile);
		fclose(OutputAssetFile);
	}
}

void BoneAsset::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn)
{
	OnDeserializing(FileIn);
}

void BoneAsset::OnSerializing(Bone* BoneIn, FILE* FileIn)
{
	// Bone Name
	const string& BoneName = BoneIn->BoneName;
	size_t BoneNameCount = BoneName.size();
	fwrite(&BoneNameCount, sizeof(size_t), 1, FileIn);
	fwrite(BoneName.c_str(), sizeof(char), BoneNameCount, FileIn);

	// Bone Idx
	fwrite(&(BoneIn->BoneIdx), sizeof(size_t), 1, FileIn);
	
	// Offset Matrix
	fwrite(&(BoneIn->OffsetMatrix), sizeof(XMMATRIX), 1, FileIn);

	// Bone Children
	size_t ChildrenCount = BoneIn->ChildrenBones.size();
	fwrite(&ChildrenCount, sizeof(size_t), 1, FileIn);
	
	for (size_t idx = 0; idx < ChildrenCount; ++idx)
	{
		OnSerializing(BoneIn->ChildrenBones[idx], FileIn);
	}
}

void BoneAsset::OnDeserializing(FILE* FileIn)
{
	// Bone Name
	string BoneName;
	size_t BoneNameCount;
	fread(&BoneNameCount, sizeof(size_t), 1, FileIn);
	BoneName.resize(BoneNameCount);
	fread(BoneName.data(), sizeof(char), BoneNameCount, FileIn);

	NameToBones.emplace(BoneName, Bone());
	Bone* DeserializingBone = &NameToBones[BoneName];
	
	TraverseDownBone(DeserializingBone);

	// Bone Idx
	fread(&DeserializingBone->BoneIdx, sizeof(size_t), 1, FileIn);
	
	// Offset Matrix
	fread(&(DeserializingBone->OffsetMatrix), sizeof(XMMATRIX), 1, FileIn);

	// Bone Children
	size_t ChildrenCount;
	fread(&ChildrenCount, sizeof(size_t), 1, FileIn);

	for (size_t idx = 0; idx < ChildrenCount; ++idx)
	{
		OnDeserializing(FileIn);
	}

	TraverseUpBone();
}

