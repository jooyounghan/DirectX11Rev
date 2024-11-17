#include "ANBTMeshAsset.h"

using namespace std;
using namespace DirectX;

ANBTMeshAsset::ANBTMeshAsset(const string& AssetNameIn, const string& AssetTypeIn)
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
}

void ANBTMeshAsset::Initialize()
{
	AMeshAsset::Initialize();
	for (auto& Tangents : TangentsPerLOD)
	{
		Tangents.VerticesBuffer.InitializeForGPU(Tangents.GetVertexCount(), Tangents.Vertices.data());
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
}

void ANBTMeshAsset::DeserializeBaseMeshData(FILE* FileIn)
{
	AMeshAsset::DeserializeBaseMeshData(FileIn);
	TangentsPerLOD.resize(LODCount);

	// Tangents
	for (auto& Tangents : TangentsPerLOD)
	{
		size_t TangentsCount;
		fread(&TangentsCount, sizeof(size_t), 1, FileIn);
		Tangents.Vertices.resize(TangentsCount);
		fread(Tangents.Vertices.data(), sizeof(XMFLOAT3), TangentsCount, FileIn);
	}
}
