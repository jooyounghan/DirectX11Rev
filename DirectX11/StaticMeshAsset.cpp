#include "StaticMeshAsset.h"
#include <stdio.h>

using namespace std;

StaticMeshAsset::StaticMeshAsset()
	: IMeshAsset("", EAssetType::None)
{
}

StaticMeshAsset::StaticMeshAsset(const string& AssetNameIn)
	: IMeshAsset(AssetNameIn + "_StaticMesh", EAssetType::StaticMesh)
{
}

StaticMeshAsset::~StaticMeshAsset()
{
}

void StaticMeshAsset::Initialize(ID3D11Device* DeviceIn)
{
	VerticesBuffer.InitializeForGPU(DeviceIn, Vertices.size(), Vertices.data());
	IndicesBuffer.InitializeForGPU(DeviceIn, Indices.size(), Indices.data());
}

void StaticMeshAsset::Serialize(const string& OutputAdditionalPath)
{
	const string OutputPath = OutputAdditionalPath.empty() ?
		AssetOutPath: AssetOutPath + OutputAdditionalPath;

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
		fwrite(Vertices.data(), sizeof(StaticVertex), VertexCount, OutputAssetFile);

		size_t IndexCount = Indices.size();
		fwrite(&IndexCount, sizeof(size_t), 1, OutputAssetFile);
		fwrite(Indices.data(), sizeof(uint32_t), IndexCount, OutputAssetFile);
		
		fclose(OutputAssetFile);
	}
}

void StaticMeshAsset::Deserialize(const string& InputFullPath, ID3D11Device* DeviceIn)
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
		fread(Vertices.data(), sizeof(StaticVertex), VertexCount, InputAssetFile);

		size_t IndexCount;
		fread(&IndexCount, sizeof(size_t), 1, InputAssetFile);
		Indices.resize(IndexCount);
		fread(Indices.data(), sizeof(uint32_t), IndexCount, InputAssetFile);

		fclose(InputAssetFile);

		Initialize(DeviceIn);
	}
}
