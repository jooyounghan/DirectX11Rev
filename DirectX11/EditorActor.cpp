#include "EditorActor.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#include "EditorCamera.h"

EditorActor::EditorActor(GraphicsPipeline* GraphicsPipelineInstance)
	: PlaceableObject(GraphicsPipelineInstance->GetDevice(), GraphicsPipelineInstance->GetDeviceContext())
{
	EditorCameraCached = AddAttachedObject<EditorCamera>(GraphicsPipelineInstance, App::GWidth, App::GHeight);

    // Test =============================================================
    EditorCameraCached->Position = SPosition4D{ 0.f, 0.f, -300.f, 1.f };
    // =============================================================
}

EditorActor::~EditorActor()
{

}

void EditorActor::OnSerialize(FILE* FileIn)
{
}

void EditorActor::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
}
