#pragma once
#include "PlaceableObject.h"
#include "HeaderHelper.h"

class GraphicsPipeline;
class IIntersectable;

class TestActor : public PlaceableObject
{
public:
	TestActor(GraphicsPipeline* GraphicsPipelineInstances);
	virtual ~TestActor();

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override {};

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

