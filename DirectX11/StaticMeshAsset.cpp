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
	VerticesBuffer.InitializeForGPU(DeviceIn, GetVertexCount(), Vertices.data());
	IndicesBuffer.InitializeForGPU(DeviceIn, GetIndexCount(), Indices.data());
}

void StaticMeshAsset::Serialize(const string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

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

void StaticMeshAsset::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn)
{
	// Vertices / Indices
	size_t VertexCount;
	fread(&VertexCount, sizeof(size_t), 1, FileIn);
	Vertices.resize(VertexCount);
	fread(Vertices.data(), sizeof(StaticVertex), VertexCount, FileIn);

	size_t IndexCount;
	fread(&IndexCount, sizeof(size_t), 1, FileIn);
	Indices.resize(IndexCount);
	fread(Indices.data(), sizeof(uint32_t), IndexCount, FileIn);

	Initialize(DeviceIn);
}
