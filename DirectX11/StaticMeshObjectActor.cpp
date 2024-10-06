#include "StaticMeshObjectActor.h"
#include "StaticMeshObject.h"

#include "IGuiModelVisitor.h"

const char* StaticMeshObjectActor::StaticMeshObjectActorIdentifier = "Static Actor";

using namespace std;

StaticMeshObjectActor::StaticMeshObjectActor(
	MapAsset* MapAssetInstance, 
	std::shared_ptr<StaticMeshAsset> MeshAssetInstanceIn
)
	: AActor(MapAssetInstance)
{
	static size_t StaticActorCount = 0;
	StaticActorCount++;
	ObjectName = StaticMeshObjectActorIdentifier + to_string(StaticActorCount);

	PlaceableKind = EPlaceableObjectKind::STATIC_MESH_ACTOR_KIND;

	StaticMeshObjectInstance = make_unique<StaticMeshObject>(MapAssetInstance, MeshAssetInstanceIn);
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

void StaticMeshObjectActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void StaticMeshObjectActor::Render()
{
	AActor::Render();
	StaticMeshObjectInstance->Render();
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
