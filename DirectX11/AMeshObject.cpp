#include "AMeshObject.h"
#include "AssetManager.h"
#include "AMeshAsset.h"

#include "MathematicalHelper.h"

using namespace std;
using namespace DirectX;

AMeshObject::AMeshObject(MapAsset* MapAssetInstance)
	: AAttachableObject(MapAssetInstance)
{
}

AMeshObject::~AMeshObject()
{
}

void AMeshObject::SetMaterialAsset(const size_t& MaterialIdx, std::shared_ptr<MaterialAsset> MaterialAssetIn)
{
	if (MaterialAssetInstances.size() > MaterialIdx)
	{
		MaterialAssetInstances[MaterialIdx] = MaterialAssetIn;
	}
}

MaterialAsset* AMeshObject::GetMaterialAssetInstance(const size_t& MaterialIdx)
{
	MaterialAsset* Result = nullptr;
	if (MaterialAssetInstances.size() > MaterialIdx)
	{
		Result = MaterialAssetInstances[MaterialIdx].get();
	}
	return Result;
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

	const float LODFloat = pow(x, (1.f / SteepLevel)) * MaxLODCount;
	return static_cast<size_t>(LODFloat);
}

void AMeshObject::Render()
{
	AAttachableObject::Render();
}

void AMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	size_t MaterialCount = MaterialAssetInstances.size();
	fwrite(&MaterialCount, sizeof(size_t), 1, FileIn);

	for (auto& MaterialAssetInstance : MaterialAssetInstances)
	{
		AAssetFile::SerializeString(MaterialAssetInstance->GetAssetName(), FileIn);
	}
}

void AMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	size_t MaterialCount;
	fread(&MaterialCount, sizeof(size_t), 1, FileIn);

	MaterialAssetInstances.resize(MaterialCount);
	for (size_t idx = 0; idx < MaterialCount; ++idx)
	{
		string MaterialAssetName;
		AAssetFile::DeserializeString(MaterialAssetName, FileIn);
		
		MaterialAssetInstances[idx] = AssetManagerIn->GetManagingMaterial(MaterialAssetName);
	}
}
