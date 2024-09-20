#pragma once
#include "APawn.h"
#include "HeaderHelper.h"
#include "imgui.h"

class IDSelectCamera;

class EditorPawn : public APawn
{
public:
	EditorPawn();
	virtual ~EditorPawn();

protected:
	IDSelectCamera* IDSelectCameraCached = nullptr;
	MakeGetter(IDSelectCameraCached);

public:
	virtual void Update(const float& DeltaTimeIn) override;
	virtual void UpdateRenderable(const bool& RenderableFlag) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override {};

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

