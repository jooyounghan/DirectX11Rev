#pragma once
#include "APlaceableObject.h"
#include "HeaderHelper.h"

class AMeshAsset;

class AActor : public APlaceableObject
{
public:
	AActor();
	virtual ~AActor();

public:
	virtual void UpdateRenderable(const bool& RenderableFlag) = 0;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) = 0;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};
