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

void Bone::AddChildBone(Bone* ChildBone)
{
	ChildBone->SetParentBone(this);
	ChildrenBones.push_back(ChildBone);
}

void Bone::OnSerialize(FILE* FileIn)
{
	// Bone Name
	size_t BoneNameCount = BoneName.size();
	fwrite(&BoneNameCount, sizeof(size_t), 1, FileIn);
	fwrite(BoneName.c_str(), sizeof(char), BoneNameCount, FileIn);

	// Bone Idx
	fwrite(&(BoneIdx), sizeof(size_t), 1, FileIn);

	// Offset Matrix
	fwrite(&(OffsetMatrix), sizeof(XMMATRIX), 1, FileIn);
}

void Bone::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Bone Name
	size_t BoneNameCount;
	fread(&BoneNameCount, sizeof(size_t), 1, FileIn);
	BoneName.resize(BoneNameCount);
	fread(BoneName.data(), sizeof(char), BoneNameCount, FileIn);

	// Bone Idx
	fread(&BoneIdx, sizeof(size_t), 1, FileIn);

	// Offset Matrix
	fread(&(OffsetMatrix), sizeof(XMMATRIX), 1, FileIn);
}

BoneAsset::BoneAsset(const std::string& AssetNameIn, bool LoadAsFile)
	: AAssetFile(LoadAsFile ? AssetNameIn + AssetSuffix[GetAssetTypeAsIndex(EAssetType::Bone)] : AssetNameIn, EAssetType::Bone)
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

		// Total Bone Count
		size_t BoneCount = NameToBones.size();
		fwrite(&BoneCount, sizeof(size_t), 1, OutputAssetFile);

		for (auto& NameToBone : NameToBones)
		{
			size_t BoneNameCount = NameToBone.first.size();
			fwrite(&BoneNameCount, sizeof(size_t), 1, OutputAssetFile);
			fwrite(NameToBone.first.c_str(), sizeof(char), BoneNameCount, OutputAssetFile);
			NameToBone.second.OnSerialize(OutputAssetFile);
		}

		// Child - Parent Relation
		for (auto& NameToBone : NameToBones)
		{
			size_t ChildBoneCount = NameToBone.second.ChildrenBones.size();
			fwrite(&ChildBoneCount, sizeof(size_t), 1, OutputAssetFile);

			for (auto& ChildBone : NameToBone.second.ChildrenBones)
			{
				size_t ChildBoneNameCount = ChildBone->BoneName.size();
				fwrite(&ChildBoneNameCount, sizeof(size_t), 1, OutputAssetFile);
				fwrite(ChildBone->BoneName.c_str(), sizeof(char), ChildBoneNameCount, OutputAssetFile);
			}
		}

		// Root Bone Name
		size_t RootBoneNameCount = RootBone->BoneName.size();
		fwrite(&RootBoneNameCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(RootBone->BoneName.c_str(), sizeof(char), RootBoneNameCount, OutputAssetFile);

		fclose(OutputAssetFile);
	}
}

void BoneAsset::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn, AssetManager* AssetManagerIn)
{
	// Total Bone Count
	size_t BoneCount;
	fread(&BoneCount, sizeof(size_t), 1, FileIn);

	for (size_t idx = 0; idx < BoneCount; ++idx)
	{
		// Bone Name
		string BoneName;
		size_t BoneNameCount;
		fread(&BoneNameCount, sizeof(size_t), 1, FileIn);
		BoneName.resize(BoneNameCount);
		fread(BoneName.data(), sizeof(char), BoneNameCount, FileIn);
		NameToBones.emplace(BoneName, Bone());

		NameToBones[BoneName].OnDeserialize(FileIn, AssetManagerIn);
	}

	for (auto& NameToBone : NameToBones)
	{
		size_t ChildBoneCount;
		fread(&ChildBoneCount, sizeof(size_t), 1, FileIn);

		for (size_t idx = 0; idx < ChildBoneCount; ++idx)
		{
			string ChildBoneName;
			size_t ChildBoneNameCount;
			fread(&ChildBoneNameCount, sizeof(size_t), 1, FileIn);
			ChildBoneName.resize(ChildBoneNameCount);
			fread(ChildBoneName.data(), sizeof(char), ChildBoneNameCount, FileIn);

			NameToBone.second.AddChildBone(&NameToBones[ChildBoneName]);
		}
	}

	string RootBoneName;
	size_t RootBoneNameCount;
	fread(&RootBoneNameCount, sizeof(size_t), 1, FileIn);
	RootBoneName.resize(RootBoneNameCount);
	fread(RootBoneName.data(), sizeof(char), RootBoneNameCount, FileIn);

	RootBone = &NameToBones[RootBoneName];
}