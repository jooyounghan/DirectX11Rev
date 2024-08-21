#pragma once
#include "PlaceableObject.h"
#include "HeaderHelper.h"

class GraphicsPipeline;
class EditorCamera;

class EditorActor : public PlaceableObject
{
public:
	EditorActor(GraphicsPipeline* GraphicsPipelineInstance);
	virtual ~EditorActor();

protected:
	EditorCamera* EditorCameraCached = nullptr;
	MakeGetter(EditorCameraCached);

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override {};

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

