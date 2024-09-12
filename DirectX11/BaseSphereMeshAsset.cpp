#include "BaseSphereMeshAsset.h"
#include "ModelHelper.h"

using namespace std;

BaseSphereMeshAsset::BaseSphereMeshAsset()
	: BaseMeshAsset("BaseSphere")
{
	ModelHelper::CreateSphere<uint32_t>(
		5, &Positions.Vertices,
		&UVTextures.Vertices,
		&Normals.Vertices,
		&Indices
	);
	Initialize();
}

BaseSphereMeshAsset::~BaseSphereMeshAsset()
{
}


BaseSphereMeshAsset* BaseSphereMeshAsset::GetInstance()
{
	static BaseSphereMeshAsset BaseSphereMeshAssetInstance;
	return &BaseSphereMeshAssetInstance;
}
