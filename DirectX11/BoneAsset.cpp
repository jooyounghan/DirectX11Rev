#include "BoneAsset.h"
#include <stdio.h>

using namespace std;
using namespace DirectX;

string BoneAsset::BoneAssetKind = "Bone";

Bone::Bone()
	: BoneName(""), BoneIndex(NULL), OffsetMatrix(XMMatrixIdentity())
{
}

Bone::Bone(
	const std::string& NameIn, 
	const size_t& BoneIdxIn, 
	const DirectX::XMMATRIX& OffsetMatrixIn
)
	: BoneName(NameIn), BoneIndex(BoneIdxIn), OffsetMatrix(OffsetMatrixIn)
{
}

Bone::~Bone()
{
}

void Bone::AddChildBone(Bone* ChildBone)
{
	ChildBone->SetParentBone(this);
	ChildrenBones.push_back(ChildBone);
}

void Bone::OnSerializeFromMap(FILE* FileIn)
{
	// Bone Name
	AAssetFile::SerializeString(BoneName, FileIn);

	// Bone Idx
	fwrite(&(BoneIndex), sizeof(size_t), 1, FileIn);

	// Offset Matrix
	fwrite(&(OffsetMatrix), sizeof(XMMATRIX), 1, FileIn);
}

void Bone::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Bone Name
	AAssetFile::DeserializeString(BoneName, FileIn);

	// Bone Idx
	fread(&BoneIndex, sizeof(size_t), 1, FileIn);

	// Offset Matrix
	fread(&(OffsetMatrix), sizeof(XMMATRIX), 1, FileIn);
	
}

BoneAsset::BoneAsset(const std::string& AssetNameIn, const bool& LoadFromAsset)
	: AAssetFile(LoadFromAsset ? AssetNameIn : AssetNameIn + "_" + BoneAsset::BoneAssetKind, BoneAsset::BoneAssetKind)
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

void BoneAsset::Serialize()
{
	FILE* OutputAssetFile = nullptr;
	string OutputAssetFilePath = DefaultOpenFileHelper(BoneAssetOutPath, OutputAssetFile);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		// Total Bone Count
		size_t BoneCount = NameToBones.size();
		fwrite(&BoneCount, sizeof(size_t), 1, OutputAssetFile);

		for (auto& NameToBone : NameToBones)
		{
			AAssetFile::SerializeString(NameToBone.first.c_str(), OutputAssetFile);
			NameToBone.second.OnSerializeFromMap(OutputAssetFile);
		}

		// Child - Parent Relation
		for (auto& NameToBone : NameToBones)
		{
			size_t ChildBoneCount = NameToBone.second.ChildrenBones.size();
			fwrite(&ChildBoneCount, sizeof(size_t), 1, OutputAssetFile);

			for (auto& ChildBone : NameToBone.second.ChildrenBones)
			{
				AAssetFile::SerializeString(ChildBone->BoneName.c_str(), OutputAssetFile);
			}
		}

		// Root Bone Name
		AAssetFile::SerializeString(RootBone->BoneName.c_str(), OutputAssetFile);

		fclose(OutputAssetFile);
	}
}

void BoneAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Total Bone Count
	size_t BoneCount;
	fread(&BoneCount, sizeof(size_t), 1, FileIn);

	for (size_t idx = 0; idx < BoneCount; ++idx)
	{
		// Bone Name
		string BoneName;
		AAssetFile::DeserializeString(BoneName, FileIn);
		NameToBones[BoneName].OnDeserializeToMap(FileIn, AssetManagerIn);
	}

	for (auto& NameToBone : NameToBones)
	{
		size_t ChildBoneCount;
		fread(&ChildBoneCount, sizeof(size_t), 1, FileIn);

		for (size_t idx = 0; idx < ChildBoneCount; ++idx)
		{
			string ChildBoneName;
			AAssetFile::DeserializeString(ChildBoneName, FileIn);
			NameToBone.second.AddChildBone(&NameToBones[ChildBoneName]);
		}
	}

	string RootBoneName;
	AAssetFile::DeserializeString(RootBoneName, FileIn);
	RootBone = &NameToBones[RootBoneName];
}
