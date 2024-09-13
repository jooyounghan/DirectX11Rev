#include "MeshObjectActor.h"
#include "MeshObject.h"
#include "IGuiModelVisitor.h"

using namespace std;

MeshObjectActor::MeshObjectActor(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn)
{
	static size_t ActorCount = 0;
	ActorCount++;
	ObjectName = "Actor" + to_string(ActorCount);

	PlaceableKind = EPlaceableObjectKind::MESH_ACTOR_KIND;

	MeshObjectInstance = make_unique<MeshObject>(MeshAssetInstanceIn);
	MeshObjectInstance->SetParent(this, PickingIDBuffer.GetBuffer());
}

MeshObjectActor::~MeshObjectActor()
{
}

void MeshObjectActor::Update(const float& DeltaTimeIn)
{
	APlaceableObject::Update(DeltaTimeIn);

	if (MeshObjectInstance != nullptr)
	{
		MeshObjectInstance->Update(DeltaTimeIn);
	}
}

void MeshObjectActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void MeshObjectActor::AcceptRenderer(ARenderer* Renderer)
{
	AcceptChildrenRenderer(Renderer);
	if (MeshObjectInstance != nullptr)
	{
		MeshObjectInstance->AcceptRenderer(Renderer);
	}
}

void MeshObjectActor::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);
	MeshObjectInstance->OnSerializeFromMap(FileIn);
}

void MeshObjectActor::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);
	MeshObjectInstance->OnDeserializeToMap(FileIn, AssetManagerIn);
}
