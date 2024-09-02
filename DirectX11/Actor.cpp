#include "Actor.h"
#include "GraphicsPipeline.h"

#include "MeshObject.h"

#include "IGuiModelVisitor.h"

using namespace std;

Actor::Actor(GraphicsPipeline* GraphicsPipelineIn)
	: APlaceableObject(GraphicsPipelineIn)
{
	Initialize(GraphicsPipelineIn, nullptr);
}

Actor::Actor(
	GraphicsPipeline* GraphicsPipelineIn,
	std::shared_ptr<AMeshAsset> MeshAssetInstanceIn
)
	: APlaceableObject(GraphicsPipelineIn)
{
	Initialize(GraphicsPipelineIn, MeshAssetInstanceIn);
}

Actor::~Actor()
{
}

void Actor::Initialize(GraphicsPipeline* GraphicsPipelineIn, std::shared_ptr<AMeshAsset> MeshAssetInstanceIn)
{
	static size_t ActorCount = 0;
	ActorCount++;
	ObjectName = "Actor" + to_string(ActorCount);

	PlaceableKind = EPlaceableObjectKind::ACTOR_KIND;

	MeshObjectInstance = make_unique<MeshObject>(GraphicsPipelineIn, MeshAssetInstanceIn);
	MeshObjectInstance->SetParentObject(this);
}

void Actor::AcceptGui(IGuiModelVisitor* GuiVisitor) 
{
	GuiVisitor->Visit(this);
}

void Actor::Update(const float& DeltaTimeIn)
{
	APlaceableObject::Update(DeltaTimeIn);
	if (MeshObjectInstance != nullptr)
	{
		MeshObjectInstance->Update(DeltaTimeIn);
	}
}

void Actor::AcceptRenderer(ARenderer* Renderer)
{
	APlaceableObject::AcceptRenderer(Renderer);
	if (MeshObjectInstance != nullptr)
	{
		MeshObjectInstance->AcceptRenderer(Renderer);
	}
}

void Actor::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);
	MeshObjectInstance->OnSerialize(FileIn);
}

void Actor::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);
	MeshObjectInstance->OnDeserialize(FileIn, AssetManagerIn);
}
