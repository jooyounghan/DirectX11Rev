#include "ANBTMeshAsset.h"

ANBTMeshAsset::ANBTMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn)
	: AMeshAsset(AssetNameIn, AssetTypeIn)
{
}

ANBTMeshAsset::~ANBTMeshAsset()
{
}

void ANBTMeshAsset::Initialize()
{
	AMeshAsset::Initialize();
	Tangents.VerticesBuffer.InitializeForGPU(Tangents.GetVertexCount(), Tangents.Vertices.data());
	Bitangents.VerticesBuffer.InitializeForGPU(Bitangents.GetVertexCount(), Bitangents.Vertices.data());
}

void ANBTMeshAsset::SerializeBaseMeshData(FILE* FileIn)
{
	AMeshAsset::SerializeBaseMeshData(FileIn);

	// Tangents
	size_t TangentsCount = Tangents.Vertices.size();
	fwrite(&TangentsCount, sizeof(size_t), 1, FileIn);
	fwrite(Tangents.Vertices.data(), sizeof(XMFLOAT3), TangentsCount, FileIn);

	// Bitangents
	size_t BitangentsCount = Bitangents.Vertices.size();
	fwrite(&BitangentsCount, sizeof(size_t), 1, FileIn);
	fwrite(Bitangents.Vertices.data(), sizeof(XMFLOAT3), BitangentsCount, FileIn);
}

void ANBTMeshAsset::DeserializeBaseMeshData(FILE* FileIn)
{
	AMeshAsset::DeserializeBaseMeshData(FileIn);

	// Tangents
	size_t TangentsCount;
	fread(&TangentsCount, sizeof(size_t), 1, FileIn);
	Tangents.Vertices.resize(TangentsCount);
	fread(Tangents.Vertices.data(), sizeof(XMFLOAT3), TangentsCount, FileIn);

	// Bitangents
	size_t BitangentsCount;
	fread(&BitangentsCount, sizeof(size_t), 1, FileIn);
	Bitangents.Vertices.resize(BitangentsCount);
	fread(Bitangents.Vertices.data(), sizeof(XMFLOAT3), BitangentsCount, FileIn);
}
