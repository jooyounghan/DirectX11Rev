#include "ANBTMeshAsset.h"

using namespace DirectX;

ANBTMeshAsset::ANBTMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn)
	: AMeshAsset(AssetNameIn, AssetTypeIn)
{
}

ANBTMeshAsset::~ANBTMeshAsset()
{
}

void ANBTMeshAsset::SetLODCount(const size_t& LODCountIn)
{
	AMeshAsset::SetLODCount(LODCountIn);
	TangentsPerLOD.resize(LODCountIn);
	BitangentsPerLOD.resize(LODCountIn);
}

void ANBTMeshAsset::Initialize()
{
	AMeshAsset::Initialize();
	for (auto& Tangents : TangentsPerLOD)
	{
		Tangents.VerticesBuffer.InitializeForGPU(Tangents.GetVertexCount(), Tangents.Vertices.data());
	}
	for (auto& Bitangents : BitangentsPerLOD)
	{
		Bitangents.VerticesBuffer.InitializeForGPU(Bitangents.GetVertexCount(), Bitangents.Vertices.data());
	}
}

void ANBTMeshAsset::SerializeBaseMeshData(FILE* FileIn)
{
	AMeshAsset::SerializeBaseMeshData(FileIn);

	// Tangents
	for (auto& Tangents : TangentsPerLOD)
	{
		size_t TangentsCount = Tangents.Vertices.size();
		fwrite(&TangentsCount, sizeof(size_t), 1, FileIn);
		fwrite(Tangents.Vertices.data(), sizeof(XMFLOAT3), TangentsCount, FileIn);
	}

	// Bitangents
	for (auto& Bitangents : BitangentsPerLOD)
	{
		size_t BitangentsCount = Bitangents.Vertices.size();
		fwrite(&BitangentsCount, sizeof(size_t), 1, FileIn);
		fwrite(Bitangents.Vertices.data(), sizeof(XMFLOAT3), BitangentsCount, FileIn);
	}
}

void ANBTMeshAsset::DeserializeBaseMeshData(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AMeshAsset::DeserializeBaseMeshData(FileIn, AssetManagerIn);
	TangentsPerLOD.resize(LODCount);
	BitangentsPerLOD.resize(LODCount);

	// Tangents
	for (auto& Tangents : TangentsPerLOD)
	{
		size_t TangentsCount;
		fread(&TangentsCount, sizeof(size_t), 1, FileIn);
		Tangents.Vertices.resize(TangentsCount);
		fread(Tangents.Vertices.data(), sizeof(XMFLOAT3), TangentsCount, FileIn);
	}

	// Bitangents
	for (auto& Bitangents : BitangentsPerLOD)
	{
		size_t BitangentsCount;
		fread(&BitangentsCount, sizeof(size_t), 1, FileIn);
		Bitangents.Vertices.resize(BitangentsCount);
		fread(Bitangents.Vertices.data(), sizeof(XMFLOAT3), BitangentsCount, FileIn);
	}
}
