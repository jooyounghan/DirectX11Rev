#include "BaseCubeMeshAsset.h"
#include "ModelHelper.h"

using namespace std;

BaseCubeMeshAsset::BaseCubeMeshAsset()
	: BaseMeshAsset("BaseCube")
{
	ModelHelper::CreateCube<uint32_t>(
		&Positions.Vertices,
		&UVTextures.Vertices,
		&Normals.Vertices,
		&Indices
	);
	Initialize();
}

BaseCubeMeshAsset::~BaseCubeMeshAsset()
{
}

BaseCubeMeshAsset* BaseCubeMeshAsset::GetInstance()
{
	static BaseCubeMeshAsset BaseCubeMeshAssetInstance;
	return &BaseCubeMeshAssetInstance;
}
