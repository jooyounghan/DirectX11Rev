#pragma once
#include "AActor.h"
#include "HeaderHelper.h"

class StaticMeshAsset;
class StaticMeshObject;

class StaticMeshObjectActor : public AActor
{
public:
	StaticMeshObjectActor(std::shared_ptr<StaticMeshAsset> MeshAssetInstanceIn);
	~StaticMeshObjectActor();

public:
	static std::string StaticMeshObjectActorKind;

protected:
	std::unique_ptr<StaticMeshObject> StaticMeshObjectInstance;
	MakeSmartPtrGetter(StaticMeshObjectInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Render(MapAsset* MapAssetIn) override final;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

