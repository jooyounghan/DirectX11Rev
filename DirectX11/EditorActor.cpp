#include "EditorActor.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

#include "EditorCamera.h"

EditorActor::EditorActor(GraphicsPipeline* GraphicsPipelineInstance)
	: APlaceableObject(GraphicsPipelineInstance)
{
	EditorCameraCached = AddAttachedObject<EditorCamera>(GraphicsPipelineInstance, App::GWidth, App::GHeight);

    AutoZeroArrayMemory(IsKeyPressed);

    // Test =============================================================
    EditorCameraCached->RelativePosition = DirectX::XMFLOAT3{ 0.f, 0.f, -300.f };
    // =============================================================
}

EditorActor::~EditorActor()
{

}

void EditorActor::Update(const float& DeltaTimeIn)
{
    if (IsKeyPressed[ImGuiKey_W])
    {
        RelativePosition.z += 200.f * DeltaTimeIn;
    }
    if (IsKeyPressed[ImGuiKey_S])
    {
        RelativePosition.z -= 200.f * DeltaTimeIn;
    }
    if (IsKeyPressed[ImGuiKey_A])
    {
        RelativePosition.x -= 200.f * DeltaTimeIn;
    }
    if (IsKeyPressed[ImGuiKey_D])
    {
        RelativePosition.x += 200.f * DeltaTimeIn;
    }

    APlaceableObject::Update(DeltaTimeIn);
}

void EditorActor::OnSerialize(FILE* FileIn)
{
    AObject::OnSerialize(FileIn);
}

void EditorActor::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
    AObject::OnDeserialize(FileIn, AssetManagerIn);
}
