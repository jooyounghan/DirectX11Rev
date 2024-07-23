#include "SkeletalMeshAsset.h"
#include <stdio.h>

using namespace std;

SkeletalMeshAsset::SkeletalMeshAsset()
	: IMeshAsset("", EAssetType::SkeletalMesh)
{
}

SkeletalMeshAsset::SkeletalMeshAsset(const std::string& AssetNameIn)
	: IMeshAsset(AssetNameIn + "_SkeletalMesh", EAssetType::SkeletalMesh)
{
}

SkeletalMeshAsset::~SkeletalMeshAsset()
{
}

void SkeletalMeshAsset::Initialize(ID3D11Device* DeviceIn)
{
	VerticesBuffer.InitializeForGPU(DeviceIn, GetVertexCount(), Vertices.data());
	IndicesBuffer.InitializeForGPU(DeviceIn, GetIndexCount(), Indices.data());
}

void SkeletalMeshAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		// Vertices / Indices
		size_t VertexCount = Vertices.size();
		fwrite(&VertexCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(Vertices.data(), sizeof(SkeletalVertex), VertexCount, OutputAssetFile);

		size_t IndexCount = Indices.size();
		fwrite(&IndexCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(Indices.data(), sizeof(uint32_t), IndexCount, OutputAssetFile);

		fclose(OutputAssetFile);
	}
}

void SkeletalMeshAsset::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn)
{
	// Vertices / Indices
	size_t VertexCount;
	fread(&VertexCount, sizeof(size_t), 1, FileIn);
	Vertices.resize(VertexCount);
	fread(Vertices.data(), sizeof(SkeletalVertex), VertexCount, FileIn);

	size_t IndexCount;
	fread(&IndexCount, sizeof(size_t), 1, FileIn);
	Indices.resize(IndexCount);
	fread(Indices.data(), sizeof(uint32_t), IndexCount, FileIn);

	Initialize(DeviceIn);
}

