#pragma once

#include "AActor.h"
#include "HeaderHelper.h"

class MeshObjectActor : public AActor
{
public:
	MeshObjectActor(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn);
	~MeshObjectActor();

protected:
	std::unique_ptr<MeshObject> MeshObjectInstance;
	MakeSmartPtrGetter(MeshObjectInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

