#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"
#include "AssetManager.h"

#include <stdio.h>

using namespace std;
using namespace DirectX;

SkeletalMeshAsset::SkeletalMeshAsset(const std::string& AssetNameIn, bool LoadFromAsset)
	: ANBTMeshAsset(LoadFromAsset ? AssetNameIn : AssetNameIn + AAssetFile::AssetTypeToSuffix[(EAssetType::SkeletalMesh)], EAssetType::SkeletalMesh)
{
}

SkeletalMeshAsset::~SkeletalMeshAsset()
{
}

void SkeletalMeshAsset::SetLODCount(const size_t& LODCountIn)
{
	ANBTMeshAsset::SetLODCount(LODCountIn);
	BlendWeightPerLOD.resize(LODCountIn);
	BlendIndexPerLOD.resize(LODCountIn);
}

std::vector<ID3D11Buffer*> SkeletalMeshAsset::GetVertexBuffers(const size_t& LODLevelIn)
{
	const size_t LODIndex = min(LODLevelIn, PositionsPerLOD.size() - 1);
	return std::vector<ID3D11Buffer*>
	{
		PositionsPerLOD[LODIndex].GetVertexBuffer(),
		UVTexturesPerLOD[LODIndex].GetVertexBuffer(),
		NormalsPerLOD[LODIndex].GetVertexBuffer(),
		TangentsPerLOD[LODIndex].GetVertexBuffer(),
		BitangentsPerLOD[LODIndex].GetVertexBuffer(),
		BlendWeightPerLOD[LODIndex].GetVertexBuffer(),
		BlendIndexPerLOD[LODIndex].GetVertexBuffer()
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
	ANBTMeshAsset::Initialize();
	for (auto& BlendWeight : BlendWeightPerLOD)
	{
		BlendWeight.VerticesBuffer.InitializeForGPU(BlendWeight.GetVertexCount(), BlendWeight.Vertices.data());
	}
	for (auto& BlendIndex : BlendIndexPerLOD)
	{
		BlendIndex.VerticesBuffer.InitializeForGPU(BlendIndex.GetVertexCount(), BlendIndex.Vertices.data());
	}	
}

void SkeletalMeshAsset::Serialize()
{
	FILE* OutputAssetFile;
	string OutputAssetFilePath = DefaultOpenFileHelper(SkeletalMeshAssetOutPath, OutputAssetFile);
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

	DeserializeBaseMeshData(FileIn, AssetManagerIn);

	Initialize();
}

void SkeletalMeshAsset::SerializeBaseMeshData(FILE* FileIn)
{
	ANBTMeshAsset::SerializeBaseMeshData(FileIn);

	// BlendWeight
	for (auto& BlendWeight : BlendWeightPerLOD)
	{
		size_t BlendWeightCount = BlendWeight.Vertices.size();
		fwrite(&BlendWeightCount, sizeof(size_t), 1, FileIn);
		fwrite(BlendWeight.Vertices.data(), sizeof(XMFLOAT4), BlendWeightCount, FileIn);
	}

	// BlendIndex
	for (auto& BlendIndex : BlendIndexPerLOD)
	{
		size_t BlendIndexCount = BlendIndex.Vertices.size();
		fwrite(&BlendIndexCount, sizeof(size_t), 1, FileIn);
		fwrite(BlendIndex.Vertices.data(), sizeof(XMINT4), BlendIndexCount, FileIn);
	}
}

void SkeletalMeshAsset::DeserializeBaseMeshData(FILE* FileIn, AssetManager* AssetManagerIn)
{
	ANBTMeshAsset::DeserializeBaseMeshData(FileIn, AssetManagerIn);
	BlendWeightPerLOD.resize(LODCount);
	BlendIndexPerLOD.resize(LODCount);

	// BlendWeight
	for (auto& BlendWeight : BlendWeightPerLOD)
	{
		size_t BlendWeightCount;
		fread(&BlendWeightCount, sizeof(size_t), 1, FileIn);
		BlendWeight.Vertices.resize(BlendWeightCount);
		fread(BlendWeight.Vertices.data(), sizeof(XMFLOAT4), BlendWeightCount, FileIn);
	}

	// BlendIndex
	for (auto& BlendIndex : BlendIndexPerLOD)
	{
		size_t BlendIndexCount;
		fread(&BlendIndexCount, sizeof(size_t), 1, FileIn);
		BlendIndex.Vertices.resize(BlendIndexCount);
		fread(BlendIndex.Vertices.data(), sizeof(XMINT4), BlendIndexCount, FileIn);
	}
}