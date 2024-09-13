#include "EditorPawn.h"

#include "GlobalVariable.h"
#include "InputEventManager.h"
#include "GraphicsPipeline.h"
#include "DefaultController.h"
#include "EditorCamera.h"

using namespace std;

EditorPawn::EditorPawn()
	: APawn()
{
    Controller = make_unique<DefaultController>();
    Controller->SetPossesdPawn(this);

	EditorCameraCached = AddAttachedObject<EditorCamera>(App::GWidth, App::GHeight);
    EditorCameraCached->SetParent(this, PickingIDBuffer.GetBuffer());
}

EditorPawn::~EditorPawn()
{

}

void EditorPawn::Update(const float& DeltaTimeIn)
{
    APlaceableObject::Update(DeltaTimeIn);
}

void EditorPawn::OnSerializeFromMap(FILE* FileIn)
{
    AObject::OnSerializeFromMap(FileIn);
}

void EditorPawn::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
    AObject::OnDeserializeToMap(FileIn, AssetManagerIn);
}
