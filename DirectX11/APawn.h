#pragma once
#include "MeshObjectActor.h"

class AController;
class InputEventManager;

class APawn : public MeshObjectActor
{
public:
	APawn();
	APawn(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn);
	virtual ~APawn();

protected:
	std::unique_ptr<AController> Controller;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

