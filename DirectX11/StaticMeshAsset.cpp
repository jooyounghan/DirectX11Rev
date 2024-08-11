#include "StaticMeshAsset.h"
#include <stdio.h>

using namespace std;

StaticMeshAsset::StaticMeshAsset()
	: AMeshAsset("", EAssetType::None)
{
}

StaticMeshAsset::StaticMeshAsset(const string& AssetNameIn, bool LoadAsFile)
	: AMeshAsset(LoadAsFile ? AssetNameIn + AssetSuffix[GetAssetTypeAsIndex(EAssetType::StaticMesh)] : AssetNameIn, EAssetType::StaticMesh)
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
		sizeof(SPosition3D),
		sizeof(SCoordinate2D),
		sizeof(SVector3D),
		sizeof(SVector3D),
		sizeof(SVector3D)
	};
}

std::vector<UINT> StaticMeshAsset::GetOffsets()
{
	return std::vector<UINT>
	{
		0, 0, 0, 0, 0
	};
}

void StaticMeshAsset::Initialize(ID3D11Device* DeviceIn)
{
	AMeshAsset::Initialize(DeviceIn);
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

void StaticMeshAsset::Deserialize(FILE* FileIn, ID3D11Device* DeviceIn)
{
	DeserializeBaseMeshData(FileIn);
	Initialize(DeviceIn);
}
