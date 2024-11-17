#include "SkeletalMeshObjectActor.h"
#include "SkeletalMeshObject.h"
#include "IGuiModelVisitor.h"

using namespace std;

string SkeletalMeshObjectActor::SkeletalMeshObjectActorKind = "Skeletal Actor";

SkeletalMeshObjectActor::SkeletalMeshObjectActor(
	std::shared_ptr<SkeletalMeshAsset> MeshAssetInstanceIn
)
	: AActor(SkeletalMeshObjectActor::SkeletalMeshObjectActorKind)
{
	static size_t SkeletalActorCount = 0;
	SkeletalActorCount++;
	ObjectName = SkeletalMeshObjectActor::SkeletalMeshObjectActorKind + to_string(SkeletalActorCount);

	SkeletalMeshObjectInstance = make_unique<SkeletalMeshObject>(MeshAssetInstanceIn);
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

void SkeletalMeshObjectActor::Render(MapAsset* MapAssetIn)
{
	AActor::Render(MapAssetIn);
	SkeletalMeshObjectInstance->Render(MapAssetIn);
}

void SkeletalMeshObjectActor::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);
	SkeletalMeshObjectInstance->OnSerialize(FileIn);
}

void SkeletalMeshObjectActor::OnDeserialize(FILE* FileIn)
{
	AObject::OnDeserialize(FileIn);
	SkeletalMeshObjectInstance->OnDeserialize(FileIn);
}
