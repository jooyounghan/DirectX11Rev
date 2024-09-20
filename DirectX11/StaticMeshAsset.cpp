#include "StaticMeshAsset.h"
#include <stdio.h>

using namespace std;


StaticMeshAsset::StaticMeshAsset(const string& AssetNameIn, bool LoadAsFile)
	: ANBTMeshAsset(LoadAsFile ? AssetNameIn : AssetNameIn + AAssetFile::AssetTypeToSuffix[(EAssetType::StaticMesh)], EAssetType::StaticMesh)
{
}

StaticMeshAsset::~StaticMeshAsset()
{
}

std::vector<ID3D11Buffer*> StaticMeshAsset::GetVertexBuffers()
{
	return std::vector<ID3D11Buffer*>
	 {
		 Positions.GetVertexBuffer(),
		 UVTextures.GetVertexBuffer(),
		 Normals.GetVertexBuffer(),
		 Tangents.GetVertexBuffer(),
		 Bitangents.GetVertexBuffer()
	 };
}

std::vector<UINT> StaticMeshAsset::GetStrides()
{
	return std::vector<UINT>
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT3)
	};
}

std::vector<UINT> StaticMeshAsset::GetOffsets()
{
	return std::vector<UINT>
	{
		0, 0, 0, 0, 0
	};
}

void StaticMeshAsset::Initialize()
{
	AMeshAsset::Initialize();
}


void StaticMeshAsset::Serialize(const string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);
		SerializeBaseMeshData(OutputAssetFile);
		
		fclose(OutputAssetFile);
	}
}

void StaticMeshAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	DeserializeBaseMeshData(FileIn);
	Initialize();
}
