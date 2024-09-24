#include "BaseMeshAsset.h"

using namespace std;

BaseMeshAsset::BaseMeshAsset(const string& AssetNameIn) 
	: AMeshAsset(AssetNameIn, EAssetType::BaseMesh)
{

}

BaseMeshAsset::~BaseMeshAsset()
{
}

vector<ID3D11Buffer*> BaseMeshAsset::GetVertexBuffers(const size_t& LODLevelIn)
{
	const size_t LODIndex = min(LODLevelIn, PositionsPerLOD.size() - 1);
	return vector<ID3D11Buffer*>
	{
		PositionsPerLOD[LODIndex].GetVertexBuffer(),
		UVTexturesPerLOD[LODIndex].GetVertexBuffer(),
		NormalsPerLOD[LODIndex].GetVertexBuffer()
	};
}

vector<UINT> BaseMeshAsset::GetStrides()
{
	return vector<UINT>
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3)
	};
}

vector<UINT> BaseMeshAsset::GetOffsets()
{
	return vector<UINT>
	{
		0, 0, 0
	};
}