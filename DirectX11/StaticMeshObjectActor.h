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

protected:
	std::unique_ptr<StaticMeshObject> StaticMeshObjectInstance;
	MakeSmartPtrGetter(StaticMeshObjectInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;
	virtual void UpdateRenderable(const bool& RenderableFlag) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};
