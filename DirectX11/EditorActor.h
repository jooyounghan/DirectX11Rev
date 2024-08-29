#pragma once
#include "APlaceableObject.h"
#include "HeaderHelper.h"
#include "imgui.h"

class EditorCamera;

class EditorActor : public APlaceableObject
{
public:
	EditorActor(GraphicsPipeline* GraphicsPipelineInstance);
	virtual ~EditorActor();

protected:
	EditorCamera* EditorCameraCached = nullptr;
	MakeGetter(EditorCameraCached);

protected:
	bool IsKeyPressed[ImGuiKey_COUNT];

public:
	inline void PressKey(ImGuiKey KeyEnum) { IsKeyPressed[KeyEnum] = true; }
	inline void ReleaseKey(ImGuiKey KeyEnum) { IsKeyPressed[KeyEnum] = false; }

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override {};

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

