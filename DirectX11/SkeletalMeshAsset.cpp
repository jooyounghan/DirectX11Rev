#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"
#include <stdio.h>

using namespace std;

SkeletalMeshAsset::SkeletalMeshAsset()
	: AMeshAsset("", EAssetType::SkeletalMesh)
{
}

SkeletalMeshAsset::SkeletalMeshAsset(const std::string& AssetNameIn, bool LoadAsFile)
	: AMeshAsset(LoadAsFile ? AssetNameIn + AssetSuffix[GetAssetTypeAsIndex(EAssetType::SkeletalMesh)] : AssetNameIn, EAssetType::SkeletalMesh)
{
}

SkeletalMeshAsset::~SkeletalMeshAsset()
{
}

void SkeletalMeshAsset::LinkBoneAsset(BoneAsset* BoneAssetIn)
{
	LinkedBoneAsset = BoneAssetIn;
	BoneAssetName = BoneAssetIn->GetAssetName();
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
		sizeof(SPosition3D),
		sizeof(SCoordinate2D),
		sizeof(SVector3D),
		sizeof(SVector3D),
		sizeof(SVector3D),
		sizeof(SVector4D),
		sizeof(SVector4D)
	};
}

std::vector<UINT> SkeletalMeshAsset::GetOffsets()
{
	return std::vector<UINT>
	{
		0, 0, 0, 0, 0, 0, 0
	};
}

void SkeletalMeshAsset::Initialize(ID3D11Device* DeviceIn)
{
	AMeshAsset::Initialize(DeviceIn);
	BlendWeight.VerticesBuffer.InitializeForGPU(DeviceIn, BlendWeight.GetVertexCount(), BlendWeight.Vertices.data());
	BlendIndex.VerticesBuffer.InitializeForGPU(DeviceIn, BlendIndex.GetVertexCount(), BlendIndex.Vertices.data());
}

void SkeletalMeshAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		// Linked Bone Name
		size_t LinkedBoneAssetNameCount;
		LinkedBoneAssetNameCount = BoneAssetName.size();
		fwrite(&LinkedBoneAssetNameCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(BoneAssetName.c_str(), sizeof(char), LinkedBoneAssetNameCount, OutputAssetFile);

		SerializeBaseMeshData(OutputAssetFile);

		// BlendWeight
		size_t BlendWeightCount = BlendWeight.Vertices.size();
		fwrite(&BlendWeightCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(BlendWeight.Vertices.data(), sizeof(SVector4D), BlendWeightCount, OutputAssetFile);

		// BlendIndex
		size_t BlendIndexCount = BlendIndex.Vertices.size();
		fwrite(&BlendIndexCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(BlendIndex.Vertices.data(), sizeof(SVector4D), BlendIndexCount, OutputAssetFile);

		fclose(OutputAssetFile);
	}
}

void SkeletalMeshAsset::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn)
{
	// Linked Bone Name
	size_t LinkedBoneAssetNameCount;
	fread(&LinkedBoneAssetNameCount, sizeof(size_t), 1, FileIn);
	BoneAssetName.resize(LinkedBoneAssetNameCount);
	fread(BoneAssetName.data(), sizeof(char), LinkedBoneAssetNameCount, FileIn);

	DeserializeBaseMeshData(FileIn);

	// BlendWeight
	size_t BlendWeightCount;
	fread(&BlendWeightCount, sizeof(size_t), 1, FileIn);
	BlendWeight.Vertices.resize(BlendWeightCount);
	fread(BlendWeight.Vertices.data(), sizeof(SVector4D), BlendWeightCount, FileIn);

	// BlendIndex
	size_t BlendIndexCount;
	fread(&BlendIndexCount, sizeof(size_t), 1, FileIn);
	BlendIndex.Vertices.resize(BlendIndexCount);
	fread(BlendIndex.Vertices.data(), sizeof(SVector4D), BlendIndexCount, FileIn);

	Initialize(DeviceIn);
}

