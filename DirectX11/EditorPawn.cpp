#include "EditorPawn.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "DefaultController.h"
#include "EditorCamera.h"

using namespace std;

EditorPawn::EditorPawn(GraphicsPipeline* GraphicsPipelineInstance)
	: APawn(GraphicsPipelineInstance)
{

    Controller = make_unique<DefaultController>();
    Controller->SetPossesdPawn(this);

    // Test =============================================================
	EditorCameraCached = AddAttachedObject<EditorCamera>(GraphicsPipelineInstance, App::GWidth, App::GHeight);
    EditorCameraCached->SetParentObject(this);
    // =============================================================
}

EditorPawn::~EditorPawn()
{

}

void EditorPawn::Update(const float& DeltaTimeIn)
{
    APlaceableObject::Update(DeltaTimeIn);
}

void EditorPawn::OnSerialize(FILE* FileIn)
{
    AObject::OnSerialize(FileIn);
}

void EditorPawn::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
    AObject::OnDeserialize(FileIn, AssetManagerIn);
}
