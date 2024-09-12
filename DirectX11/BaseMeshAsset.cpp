#include "BaseMeshAsset.h"

BaseMeshAsset::BaseMeshAsset(const std::string& AssetNameIn) 
	: AMeshAsset(AssetNameIn, EAssetType::BaseMesh)
{

}

BaseMeshAsset::~BaseMeshAsset()
{
}

std::vector<ID3D11Buffer*> BaseMeshAsset::GetVertexBuffers()
{
	return std::vector<ID3D11Buffer*>
	{
		Positions.GetVertexBuffer(),
		UVTextures.GetVertexBuffer(),
		Normals.GetVertexBuffer()
	};
}

std::vector<UINT> BaseMeshAsset::GetStrides()
{
	return std::vector<UINT>
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3)
	};
}

std::vector<UINT> BaseMeshAsset::GetOffsets()
{
	return std::vector<UINT>
	{
		0, 0, 0
	};
}