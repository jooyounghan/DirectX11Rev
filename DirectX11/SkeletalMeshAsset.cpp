#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"
#include "AssetManager.h"

#include <stdio.h>

using namespace std;

SkeletalMeshAsset::SkeletalMeshAsset(const std::string& AssetNameIn, bool LoadAsFile)
	: ANBTMeshAsset(LoadAsFile ? AssetNameIn : AssetNameIn + AAssetFile::AssetTypeToSuffix[(EAssetType::SkeletalMesh)], EAssetType::SkeletalMesh)
{
}

SkeletalMeshAsset::~SkeletalMeshAsset()
{
}

std::vector<ID3D11Buffer*> SkeletalMeshAsset::GetVertexBuffers()
{
	return std::vector<ID3D11Buffer*>
	{
		Positions.GetVertexBuffer(),
		UVTextures.GetVertexBuffer(),
		Normals.GetVertexBuffer(),
		Tangents.GetVertexBuffer(),
		Bitangents.GetVertexBuffer(),
		BlendWeight.GetVertexBuffer(),
		BlendIndex.GetVertexBuffer()
	};
}

std::vector<UINT> SkeletalMeshAsset::GetStrides()
{
	return std::vector<UINT>
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT4),
		sizeof(XMINT4)
	};
}

std::vector<UINT> SkeletalMeshAsset::GetOffsets()
{
	return std::vector<UINT>
	{
		0, 0, 0, 0, 0, 0, 0
	};
}

void SkeletalMeshAsset::Initialize()
{
	AMeshAsset::Initialize();
	BlendWeight.VerticesBuffer.InitializeForGPU(BlendWeight.GetVertexCount(), BlendWeight.Vertices.data());
	BlendIndex.VerticesBuffer.InitializeForGPU(BlendIndex.GetVertexCount(), BlendIndex.Vertices.data());
}

void SkeletalMeshAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		// Linked Bone Name
		const string& BoneAssetName = LinkedBoneAsset->GetAssetName();
		size_t LinkedBoneAssetNameCount = 0;
		if (LinkedBoneAsset != nullptr)
		{
			LinkedBoneAssetNameCount = BoneAssetName.size();
		}
		fwrite(&LinkedBoneAssetNameCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(BoneAssetName.c_str(), sizeof(char), LinkedBoneAssetNameCount, OutputAssetFile);

		SerializeBaseMeshData(OutputAssetFile);

		// BlendWeight
		size_t BlendWeightCount = BlendWeight.Vertices.size();
		fwrite(&BlendWeightCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(BlendWeight.Vertices.data(), sizeof(XMFLOAT4), BlendWeightCount, OutputAssetFile);

		// BlendIndex
		size_t BlendIndexCount = BlendIndex.Vertices.size();
		fwrite(&BlendIndexCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(BlendIndex.Vertices.data(), sizeof(XMINT4), BlendIndexCount, OutputAssetFile);

		fclose(OutputAssetFile);
	}
}

void SkeletalMeshAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	// Linked Bone Name
	string BoneAssetName;
	size_t LinkedBoneAssetNameCount;
	fread(&LinkedBoneAssetNameCount, sizeof(size_t), 1, FileIn);
	BoneAssetName.resize(LinkedBoneAssetNameCount);
	fread(BoneAssetName.data(), sizeof(char), LinkedBoneAssetNameCount, FileIn);

	std::shared_ptr<BoneAsset> BoneAssetFile = AssetManagerIn->GetManagingBone(BoneAssetName);
	LinkedBoneAsset = BoneAssetFile;

	DeserializeBaseMeshData(FileIn);

	// BlendWeight
	size_t BlendWeightCount;
	fread(&BlendWeightCount, sizeof(size_t), 1, FileIn);
	BlendWeight.Vertices.resize(BlendWeightCount);
	fread(BlendWeight.Vertices.data(), sizeof(XMFLOAT4), BlendWeightCount, FileIn);

	// BlendIndex
	size_t BlendIndexCount;
	fread(&BlendIndexCount, sizeof(size_t), 1, FileIn);
	BlendIndex.Vertices.resize(BlendIndexCount);
	fread(BlendIndex.Vertices.data(), sizeof(XMINT4), BlendIndexCount, FileIn);

	Initialize();
}