#include "EditorPawn.h"

#include "GlobalVariable.h"
#include "InputEventManager.h"

#include "IGuiModelVisitor.h"

#include "GraphicsPipeline.h"
#include "DefaultController.h"
#include "Camera.h"

using namespace std;

EditorPawn::EditorPawn(MapAsset* MapAssetInstance)
	: APawn(MapAssetInstance)
{
    Controller = make_unique<DefaultController>();
    Controller->SetPossesdPawn(this);

    CameraInstance = make_unique<Camera>(MapAssetInstance, App::GWidth, App::GHeight);
    CameraInstance->SetParent(this, PickingIDBuffer.GetBuffer());
}

EditorPawn::~EditorPawn()
{

}

void EditorPawn::Update(const float& DeltaTimeIn)
{
    APlaceableObject::Update(DeltaTimeIn);
    CameraInstance->Update(DeltaTimeIn);
}

void EditorPawn::Render()
{
    AActor::Render();
}

void EditorPawn::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
    GuiVisitor->Visit(this);
}

void EditorPawn::OnSerializeFromMap(FILE* FileIn)
{
    AObject::OnSerializeFromMap(FileIn);
    CameraInstance->OnSerializeFromMap(FileIn);
}

void EditorPawn::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
    AObject::OnDeserializeToMap(FileIn, AssetManagerIn);
    CameraInstance->OnDeserializeToMap(FileIn, AssetManagerIn);
}
