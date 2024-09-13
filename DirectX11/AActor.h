#pragma once
#include "APlaceableObject.h"
#include "HeaderHelper.h"

class AMeshAsset;
class MeshObject;

class AActor : public APlaceableObject
{
public:
	AActor();
	virtual ~AActor();

public:
	virtual void AcceptRenderer(ARenderer* Renderer) = 0;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) = 0;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

