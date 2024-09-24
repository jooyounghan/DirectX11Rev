#include "BaseSphereMeshAsset.h"
#include "ModelHelper.h"

using namespace std;

BaseSphereMeshAsset::BaseSphereMeshAsset()
	: BaseMeshAsset("BaseSphere")
{
	SetLODCount(1);

	ModelHelper::CreateSphere<uint32_t>(
		5, &PositionsPerLOD[0].Vertices,
		&UVTexturesPerLOD[0].Vertices,
		&NormalsPerLOD[0].Vertices,
		&IndicesPerLOD[0].Indices
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
