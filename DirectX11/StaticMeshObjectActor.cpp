#include "StaticMeshObjectActor.h"
#include "StaticMeshObject.h"
#include "IGuiModelVisitor.h"

using namespace std;

StaticMeshObjectActor::StaticMeshObjectActor(std::shared_ptr<StaticMeshAsset> MeshAssetInstanceIn)
{
	static size_t StaticActorCount = 0;
	StaticActorCount++;
	ObjectName = "Static Actor" + to_string(StaticActorCount);

	PlaceableKind = EPlaceableObjectKind::STATIC_MESH_ACTOR_KIND;

	StaticMeshObjectInstance = make_unique<StaticMeshObject>(MeshAssetInstanceIn);
	StaticMeshObjectInstance->SetParent(this, PickingIDBuffer.GetBuffer());
}

StaticMeshObjectActor::~StaticMeshObjectActor() {}

void StaticMeshObjectActor::Update(const float& DeltaTimeIn)
{
	APlaceableObject::Update(DeltaTimeIn);

	if (StaticMeshObjectInstance != nullptr)
	{
		StaticMeshObjectInstance->Update(DeltaTimeIn);
	}
}

void StaticMeshObjectActor::UpdateRenderable(const bool& RenderableFlag)
{
	IsRenderable = RenderableFlag;
	StaticMeshObjectInstance->UpdateRenderable(RenderableFlag);
	for (unique_ptr<AAttachableObject>& AttachedObject : AttachedChildrenObjects)
	{
		AttachedObject->UpdateRenderable(RenderableFlag);
	}
}

void StaticMeshObjectActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void StaticMeshObjectActor::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);
	StaticMeshObjectInstance->OnSerializeFromMap(FileIn);
}

void StaticMeshObjectActor::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);
	StaticMeshObjectInstance->OnDeserializeToMap(FileIn, AssetManagerIn);
}
