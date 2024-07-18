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
	const string OutputPath = OutputAdditionalPath.empty() ?
		AssetOutPath : AssetOutPath + OutputAdditionalPath;

	const string OutputFullPath = OutputAdditionalPath.empty() ?
		AssetOutPath + AssetName + AssetExtension : AssetOutPath + OutputAdditionalPath + AssetName + AssetExtension;

	CreateDirectoryA(OutputPath.c_str(), NULL);

	FILE* OutputAssetFile = nullptr;
	errno_t result = fopen_s(&OutputAssetFile, OutputFullPath.c_str(), "wb");

	if (OutputAssetFile != nullptr)
	{
		// Asset Name
		size_t AssetNameSize = AssetName.size();
		fwrite(&AssetNameSize, sizeof(size_t), 1, OutputAssetFile);
		fwrite(AssetName.c_str(), sizeof(char), AssetNameSize, OutputAssetFile);

		// Asset Type
		fwrite(&AssetType, sizeof(AssetType), 1, OutputAssetFile);

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

void SkeletalMeshAsset::Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn)
{
	FILE* InputAssetFile = nullptr;
	fopen_s(&InputAssetFile, InputFullPath.c_str(), "rb");

	if (InputAssetFile != nullptr)
	{
		// Asset Name
		size_t AssetNameSize;
		fread(&AssetNameSize, sizeof(size_t), 1, InputAssetFile);
		AssetName.resize(AssetNameSize);
		fread(AssetName.data(), sizeof(char), AssetNameSize, InputAssetFile);

		// Asset Type
		fread(&AssetType, sizeof(AssetType), 1, InputAssetFile);

		// Vertices / Indices
		size_t VertexCount;
		fread(&VertexCount, sizeof(size_t), 1, InputAssetFile);
		Vertices.resize(VertexCount);
		fread(Vertices.data(), sizeof(SkeletalVertex), VertexCount, InputAssetFile);

		size_t IndexCount;
		fread(&IndexCount, sizeof(size_t), 1, InputAssetFile);
		Indices.resize(IndexCount);
		fread(Indices.data(), sizeof(uint32_t), IndexCount, InputAssetFile);

		fclose(InputAssetFile);

		Initialize(DeviceIn);
	}
}

