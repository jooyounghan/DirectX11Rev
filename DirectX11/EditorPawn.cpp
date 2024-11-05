#include "EditorPawn.h"

#include "GlobalVariable.h"
#include "InputEventManager.h"

#include "IGuiModelVisitor.h"

#include "GraphicsPipeline.h"
#include "DefaultController.h"

#include "SDRCamera.h"

using namespace std;

string EditorPawn::EditorPawnKind = "Editor Pawn";

EditorPawn::EditorPawn(MapAsset* MapAssetInstance)
	: APawn(MapAssetInstance, EditorPawn::EditorPawnKind)
{
    Controller = make_unique<DefaultController>();
    Controller->SetPossesdPawn(this);

    CameraInstance = make_unique<SDRCamera>(MapAssetInstance, App::GWidth, App::GHeight);
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

void EditorPawn::OnSerialize(FILE* FileIn)
{
    AObject::OnSerialize(FileIn);
    CameraInstance->OnSerialize(FileIn);
}

void EditorPawn::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
    AObject::OnDeserialize(FileIn, AssetManagerIn);
    CameraInstance->OnDeserialize(FileIn, AssetManagerIn);
}
