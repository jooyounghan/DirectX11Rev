#include "AMeshAsset.h"

AMeshAsset::AMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn)
	: AAssetFile(AssetNameIn, AssetTypeIn)
{
}

AMeshAsset::~AMeshAsset()
{
}

void AMeshAsset::Initialize(ID3D11Device* DeviceIn)
{
	Positions.VerticesBuffer.InitializeForGPU(DeviceIn, Positions.GetVertexCount(), Positions.Vertices.data());
	UVTextures.VerticesBuffer.InitializeForGPU(DeviceIn, UVTextures.GetVertexCount(), UVTextures.Vertices.data());
	Normals.VerticesBuffer.InitializeForGPU(DeviceIn, Normals.GetVertexCount(), Normals.Vertices.data());
	Tangents.VerticesBuffer.InitializeForGPU(DeviceIn, Tangents.GetVertexCount(), Tangents.Vertices.data());
	Bitangents.VerticesBuffer.InitializeForGPU(DeviceIn, Bitangents.GetVertexCount(), Bitangents.Vertices.data());

	IndicesBuffer.InitializeForGPU(DeviceIn, GetIndexCount(), Indices.data());
}

void AMeshAsset::SerializeBaseMeshData(FILE* FileIn)
{
	// Positions
	size_t PositionCount = Positions.Vertices.size();
	fwrite(&PositionCount, sizeof(size_t), 1, FileIn);
	fwrite(Positions.Vertices.data(), sizeof(SPosition3D), PositionCount, FileIn);

	// UVTextures
	size_t UVTexturesCount = UVTextures.Vertices.size();
	fwrite(&UVTexturesCount, sizeof(size_t), 1, FileIn);
	fwrite(UVTextures.Vertices.data(), sizeof(SCoordinate2D), UVTexturesCount, FileIn);

	// Normals
	size_t NormalsCount = Normals.Vertices.size();
	fwrite(&NormalsCount, sizeof(size_t), 1, FileIn);
	fwrite(Normals.Vertices.data(), sizeof(SVector3D), NormalsCount, FileIn);

	// Tangents
	size_t TangentsCount = Tangents.Vertices.size();
	fwrite(&TangentsCount, sizeof(size_t), 1, FileIn);
	fwrite(Tangents.Vertices.data(), sizeof(SVector3D), TangentsCount, FileIn);

	// Bitangents
	size_t BitangentsCount = Bitangents.Vertices.size();
	fwrite(&BitangentsCount, sizeof(size_t), 1, FileIn);
	fwrite(Bitangents.Vertices.data(), sizeof(SVector3D), BitangentsCount, FileIn);

	// Indices
	size_t IndexCount = Indices.size();
	fwrite(&IndexCount, sizeof(size_t), 1, FileIn);
	fwrite(Indices.data(), sizeof(uint32_t), IndexCount, FileIn);
}

void AMeshAsset::DeserializeBaseMeshData(FILE* FileIn)
{
	// Positions
	size_t PositionsCount;
	fread(&PositionsCount, sizeof(size_t), 1, FileIn);
	Positions.Vertices.resize(PositionsCount);
	fread(Positions.Vertices.data(), sizeof(SPosition3D), PositionsCount, FileIn);

	// UVTextures
	size_t UVTexturesCount;
	fread(&UVTexturesCount, sizeof(size_t), 1, FileIn);
	UVTextures.Vertices.resize(UVTexturesCount);
	fread(UVTextures.Vertices.data(), sizeof(SCoordinate2D), UVTexturesCount, FileIn);

	// Normals
	size_t NormalsCount;
	fread(&NormalsCount, sizeof(size_t), 1, FileIn);
	Normals.Vertices.resize(NormalsCount);
	fread(Normals.Vertices.data(), sizeof(SVector3D), NormalsCount, FileIn);

	// Tangents
	size_t TangentsCount;
	fread(&TangentsCount, sizeof(size_t), 1, FileIn);
	Tangents.Vertices.resize(TangentsCount);
	fread(Tangents.Vertices.data(), sizeof(SVector3D), TangentsCount, FileIn);

	// Bitangents
	size_t BitangentsCount;
	fread(&BitangentsCount, sizeof(size_t), 1, FileIn);
	Bitangents.Vertices.resize(BitangentsCount);
	fread(Bitangents.Vertices.data(), sizeof(SVector3D), BitangentsCount, FileIn);

	// Indices
	size_t IndexCount;
	fread(&IndexCount, sizeof(size_t), 1, FileIn);
	Indices.resize(IndexCount);
	fread(Indices.data(), sizeof(uint32_t), IndexCount, FileIn);
}
