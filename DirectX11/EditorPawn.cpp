#include "EditorPawn.h"

#include "GlobalVariable.h"
#include "InputEventManager.h"
#include "GraphicsPipeline.h"
#include "DefaultController.h"
#include "IDSelectCamera.h"

using namespace std;

EditorPawn::EditorPawn()
	: APawn()
{
    Controller = make_unique<DefaultController>();
    Controller->SetPossesdPawn(this);

	IDSelectCameraCached = AddAttachedObject<IDSelectCamera>(App::GWidth, App::GHeight);
    IDSelectCameraCached->SetParent(this, PickingIDBuffer.GetBuffer());
}

EditorPawn::~EditorPawn()
{

}

void EditorPawn::Update(const float& DeltaTimeIn)
{
    APlaceableObject::Update(DeltaTimeIn);
}

void EditorPawn::UpdateRenderable(const bool& RenderableFlag)
{
    APlaceableObject::UpdateRenderable(RenderableFlag);
}

void EditorPawn::OnSerializeFromMap(FILE* FileIn)
{
    AObject::OnSerializeFromMap(FileIn);
}

void EditorPawn::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
    AObject::OnDeserializeToMap(FileIn, AssetManagerIn);
}
