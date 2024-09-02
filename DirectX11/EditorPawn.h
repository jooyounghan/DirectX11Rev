#pragma once
#include "APawn.h"
#include "HeaderHelper.h"
#include "imgui.h"

class EditorCamera;

class EditorPawn : public APawn
{
public:
	EditorPawn(GraphicsPipeline* GraphicsPipelineInstance);
	virtual ~EditorPawn();

protected:
	EditorCamera* EditorCameraCached = nullptr;
	MakeGetter(EditorCameraCached);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override {};

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

