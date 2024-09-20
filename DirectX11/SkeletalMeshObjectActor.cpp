#include "SkeletalMeshObjectActor.h"
#include "SkeletalMeshObject.h"
#include "IGuiModelVisitor.h"

using namespace std;

SkeletalMeshObjectActor::SkeletalMeshObjectActor(
	MapAsset* MapAssetInstance, 
	std::shared_ptr<SkeletalMeshAsset> MeshAssetInstanceIn
)
	: AActor(MapAssetInstance)
{
	static size_t SkeletalActorCount = 0;
	SkeletalActorCount++;
	ObjectName = "Skeletal Actor" + to_string(SkeletalActorCount);

	PlaceableKind = EPlaceableObjectKind::SKELETAL_MESH_ACTOR_KIND;

	SkeletalMeshObjectInstance = make_unique<SkeletalMeshObject>(MapAssetInstance, MeshAssetInstanceIn);
	SkeletalMeshObjectInstance->SetParent(this, PickingIDBuffer.GetBuffer());

}

SkeletalMeshObjectActor::~SkeletalMeshObjectActor()
{
}

void SkeletalMeshObjectActor::Update(const float& DeltaTimeIn)
{
	APlaceableObject::Update(DeltaTimeIn);

	if (SkeletalMeshObjectInstance != nullptr)
	{
		SkeletalMeshObjectInstance->Update(DeltaTimeIn);
	}
}

void SkeletalMeshObjectActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void SkeletalMeshObjectActor::Render()
{
	AActor::Render();
	SkeletalMeshObjectInstance->Render();
}

void SkeletalMeshObjectActor::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);
	SkeletalMeshObjectInstance->OnSerializeFromMap(FileIn);
}

void SkeletalMeshObjectActor::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);
	SkeletalMeshObjectInstance->OnDeserializeToMap(FileIn, AssetManagerIn);
}
