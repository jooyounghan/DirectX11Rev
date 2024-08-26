#include "EditorActor.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#include "EditorCamera.h"

EditorActor::EditorActor(GraphicsPipeline* GraphicsPipelineInstance)
	: PlaceableObject(GraphicsPipelineInstance)
{
	EditorCameraCached = AddAttachedObject<EditorCamera>(GraphicsPipelineInstance, App::GWidth, App::GHeight);

    // Test =============================================================
    EditorCameraCached->RelativePosition = SPosition4D{ 0.f, 0.f, -300.f, 1.f };
    // =============================================================
}

EditorActor::~EditorActor()
{

}

void EditorActor::OnSerialize(FILE* FileIn)
{
    AObject::OnSerialize(FileIn);
}

void EditorActor::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
    AObject::OnDeserialize(FileIn, AssetManagerIn);
}
