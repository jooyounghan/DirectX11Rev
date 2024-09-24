#include "BaseCubeMeshAsset.h"
#include "ModelHelper.h"

using namespace std;

BaseCubeMeshAsset::BaseCubeMeshAsset()
	: BaseMeshAsset("BaseCube")
{
	SetLODCount(1);

	ModelHelper::CreateCube<uint32_t>(
		&PositionsPerLOD[0].Vertices,
		&UVTexturesPerLOD[0].Vertices,
		&NormalsPerLOD[0].Vertices,
		&IndicesPerLOD[0].Indices
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
