#pragma once
#include "AActor.h"

class AController;
class InputEventManager;

class APawn : public AActor
{
public:
	APawn(MapAsset* MapAssetInstance);
	virtual ~APawn();

protected:
	std::unique_ptr<AController> Controller;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Render() = 0;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) = 0;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

