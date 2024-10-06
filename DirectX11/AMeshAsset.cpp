#include "AMeshAsset.h"
#include "MaterialAsset.h"
#include "AssetManager.h"

using namespace std;
using namespace DirectX;

AMeshAsset::AMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn)
	: AAssetFile(AssetNameIn, AssetTypeIn)
{
}

AMeshAsset::~AMeshAsset()
{
}

void AMeshAsset::SetLODCount(const size_t& LODCountIn)
{
	LODCount = LODCountIn;
	PositionsPerLOD.resize(LODCountIn);
	UVTexturesPerLOD.resize(LODCountIn);
	NormalsPerLOD.resize(LODCountIn);
	IndicesPerLOD.resize(LODCountIn);
}

ID3D11Buffer* AMeshAsset::GetIndexBuffer(const size_t& LODLevelIn)
{
	if (IndicesPerLOD.size() > LODLevelIn)
	{
		return { IndicesPerLOD[LODLevelIn].GetIndexBuffer() };
	}
	else
	{
		return nullptr;
	}
}

UINT AMeshAsset::GetIndexCount(const size_t& LODLevelIn)
{
	if (IndicesPerLOD.size() > LODLevelIn)
	{
		return { IndicesPerLOD[LODLevelIn].GetIndexCount() };
	}
	else
	{
		return NULL;
	}
}

vector<ID3D11Buffer*> AMeshAsset::GetPositionBuffer(const size_t& LODLevelIn)
{
	if (PositionsPerLOD.size() > LODLevelIn)
	{
		return { PositionsPerLOD[LODLevelIn].GetVertexBuffer() };
	}
	else
	{
		return vector<ID3D11Buffer*>();
	}
}


void AMeshAsset::Initialize()
{
	for (auto& Positions : PositionsPerLOD)
	{
		Positions.VerticesBuffer.InitializeForGPU(Positions.GetVertexCount(), Positions.Vertices.data());
	}
	for (auto& UVTextures : UVTexturesPerLOD)
	{
		UVTextures.VerticesBuffer.InitializeForGPU(UVTextures.GetVertexCount(), UVTextures.Vertices.data());
	}
	for (auto& Normals : NormalsPerLOD)
	{
		Normals.VerticesBuffer.InitializeForGPU(Normals.GetVertexCount(), Normals.Vertices.data());
	}
	for (auto& Indices : IndicesPerLOD)
	{
		Indices.IndicesBuffer.InitializeForGPU(Indices.GetIndexCount(), Indices.Indices.data());
	}

}

void AMeshAsset::SerializeBaseMeshData(FILE* FileIn)
{
	size_t MeshCount = DefaultMaterialAssets.size();
	fwrite(&MeshCount, sizeof(size_t), 1, FileIn);
	
	for (size_t MaterialIdx = 0; MaterialIdx < MeshCount; ++MaterialIdx)
	{
		const shared_ptr<MaterialAsset>& CurrentMaterialAsset = DefaultMaterialAssets[MaterialIdx];
		SerializeString(CurrentMaterialAsset != nullptr ? CurrentMaterialAsset->GetAssetName() : "", FileIn);
	}

	fwrite(&LODCount, sizeof(size_t), 1, FileIn);

	// Positions
	for (auto& Positions : PositionsPerLOD)
	{
		size_t PositionCount = Positions.Vertices.size();
		fwrite(&PositionCount, sizeof(size_t), 1, FileIn);
		fwrite(Positions.Vertices.data(), sizeof(XMFLOAT3), PositionCount, FileIn);
	}

	// UVTextures
	for (auto& UVTextures : UVTexturesPerLOD)
	{
		size_t UVTexturesCount = UVTextures.Vertices.size();
		fwrite(&UVTexturesCount, sizeof(size_t), 1, FileIn);
		fwrite(UVTextures.Vertices.data(), sizeof(XMFLOAT2), UVTexturesCount, FileIn);
	}

	// Normals
	for (auto& Normals : NormalsPerLOD)
	{
		size_t NormalsCount = Normals.Vertices.size();
		fwrite(&NormalsCount, sizeof(size_t), 1, FileIn);
		fwrite(Normals.Vertices.data(), sizeof(XMFLOAT3), NormalsCount, FileIn);
	}

	// Indices
	for (auto& Indices : IndicesPerLOD)
	{
		size_t IndicesCount = Indices.Indices.size();
		fwrite(&IndicesCount, sizeof(size_t), 1, FileIn);
		fwrite(Indices.Indices.data(), sizeof(uint32_t), IndicesCount, FileIn);
	}
}

void AMeshAsset::DeserializeBaseMeshData(FILE* FileIn, AssetManager* AssetManagerIn)
{
	size_t MeshCount;
	fread(&MeshCount, sizeof(size_t), 1, FileIn);

	DefaultMaterialAssets.resize(MeshCount);
	for (size_t MaterialIdx = 0; MaterialIdx < MeshCount; ++MaterialIdx)
	{
		string MaterialName;
		DeserializeString(MaterialName, FileIn);
		DefaultMaterialAssets[MaterialIdx] = AssetManagerIn->GetManagingMaterial(MaterialName);
	}

	fread(&LODCount, sizeof(size_t), 1, FileIn);

	PositionsPerLOD.resize(LODCount);
	UVTexturesPerLOD.resize(LODCount);
	NormalsPerLOD.resize(LODCount);
	IndicesPerLOD.resize(LODCount);

	// Positions
	for (auto& Positions : PositionsPerLOD)
	{
		size_t PositionsCount;
		fread(&PositionsCount, sizeof(size_t), 1, FileIn);
		Positions.Vertices.resize(PositionsCount);
		fread(Positions.Vertices.data(), sizeof(XMFLOAT3), PositionsCount, FileIn);
	}

	// UVTextures
	for (auto& UVTextures : UVTexturesPerLOD)
	{
		size_t UVTexturesCount;
		fread(&UVTexturesCount, sizeof(size_t), 1, FileIn);
		UVTextures.Vertices.resize(UVTexturesCount);
		fread(UVTextures.Vertices.data(), sizeof(XMFLOAT2), UVTexturesCount, FileIn);
	}

	// Normals
	for (auto& Normals : NormalsPerLOD)
	{
		size_t NormalsCount;
		fread(&NormalsCount, sizeof(size_t), 1, FileIn);
		Normals.Vertices.resize(NormalsCount);
		fread(Normals.Vertices.data(), sizeof(XMFLOAT3), NormalsCount, FileIn);
	}

	// Indices
	for (auto& Indices : IndicesPerLOD)
	{
		size_t IndexCount;
		fread(&IndexCount, sizeof(size_t), 1, FileIn);
		Indices.Indices.resize(IndexCount);
		fread(Indices.Indices.data(), sizeof(uint32_t), IndexCount, FileIn);
	}
}
