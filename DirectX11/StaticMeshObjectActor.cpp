#include "StaticMeshObjectActor.h"
#include "StaticMeshObject.h"

#include "IGuiModelVisitor.h"

using namespace std;

string StaticMeshObjectActor::StaticMeshObjectActorKind = "Static Actor";

StaticMeshObjectActor::StaticMeshObjectActor(
	MapAsset* MapAssetInstance, 
	std::shared_ptr<StaticMeshAsset> MeshAssetInstanceIn
)
	: AActor(MapAssetInstance, StaticMeshObjectActor::StaticMeshObjectActorKind)
{
	static size_t StaticActorCount = 0;
	StaticActorCount++;
	ObjectName = StaticMeshObjectActorKind + to_string(StaticActorCount);

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

void StaticMeshObjectActor::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);
	StaticMeshObjectInstance->OnSerialize(FileIn);
}

void StaticMeshObjectActor::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);
	StaticMeshObjectInstance->OnDeserialize(FileIn, AssetManagerIn);
}
