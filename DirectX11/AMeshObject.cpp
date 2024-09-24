#include "AMeshObject.h"
#include "AssetManager.h"
#include "AMeshAsset.h"

#include "MathematicalHelper.h"

using namespace std;

AMeshObject::AMeshObject(MapAsset* MapAssetInstance)
	: AAttachableObject(MapAssetInstance)
{
}

AMeshObject::~AMeshObject()
{
}

size_t AMeshObject::GetLODLevel(
	const XMFLOAT3& From, 
	const float& MaxDistance, 
	const size_t& MaxLODCount, 
	const size_t SteepLevel
)
{
	const XMFLOAT3 SkeletalMeshObjectPosition = GetAbsolutePosition();
	const XMFLOAT3 DeltaPosition = XMFLOAT3(
		From.x - SkeletalMeshObjectPosition.x,
		From.y - SkeletalMeshObjectPosition.y,
		From.z - SkeletalMeshObjectPosition.z
	);

	const float Distance = sqrt(MathematicalHelper::InnerProduct(DeltaPosition, DeltaPosition));
	const float x = min(MaxDistance, Distance) / MaxDistance;

	return static_cast<size_t>(((log10(x + pow(10, -static_cast<double>(SteepLevel))) + SteepLevel) / SteepLevel) * MaxLODCount);
}

void AMeshObject::Render()
{
	AAttachableObject::Render();
}
