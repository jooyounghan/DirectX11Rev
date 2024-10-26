#include "StaticMeshAsset.h"
#include <stdio.h>

using namespace std;
using namespace DirectX;

string StaticMeshAsset::StaticMeshAssetKind = "StaticMesh";

StaticMeshAsset::StaticMeshAsset(const string& AssetNameIn, bool LoadFromAsset)
	: ANBTMeshAsset(LoadFromAsset ? AssetNameIn : AssetNameIn + "_" + StaticMeshAsset::StaticMeshAssetKind, StaticMeshAsset::StaticMeshAssetKind)
{
}

StaticMeshAsset::~StaticMeshAsset()
{
}

std::vector<ID3D11Buffer*> StaticMeshAsset::GetVertexBuffers(const size_t& LODLevelIn)
{
	const size_t LODIndex = min(LODLevelIn, PositionsPerLOD.size() - 1);
	return std::vector<ID3D11Buffer*>
	 {
		 PositionsPerLOD[LODIndex].GetVertexBuffer(),
		 UVTexturesPerLOD[LODIndex].GetVertexBuffer(),
		 NormalsPerLOD[LODIndex].GetVertexBuffer(),
		 TangentsPerLOD[LODIndex].GetVertexBuffer(),
	 };
}

std::vector<UINT> StaticMeshAsset::GetStrides()
{
	return std::vector<UINT>
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3)
	};
}

std::vector<UINT> StaticMeshAsset::GetOffsets()
{
	return std::vector<UINT>
	{
		0, 0, 0, 0
	};
}

void StaticMeshAsset::Initialize()
{
	ANBTMeshAsset::Initialize();
}

void StaticMeshAsset::Serialize()
{
	FILE* OutputAssetFile;
	string OutputAssetFilePath = DefaultOpenFileHelper(StaticMeshAssetOutPath, OutputAssetFile);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);
		SerializeBaseMeshData(OutputAssetFile);
		
		fclose(OutputAssetFile);
	}
}

void StaticMeshAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	DeserializeBaseMeshData(FileIn, AssetManagerIn);
	Initialize();
}