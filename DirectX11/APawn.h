#pragma once
#include "AActor.h"

class AController;
class InputEventManager;

class APawn : public AActor
{
public:
	APawn();
	virtual ~APawn();

protected:
	std::unique_ptr<AController> Controller;

public:
	virtual void UpdateRenderable(const bool& RenderableFlag) = 0;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) = 0;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

