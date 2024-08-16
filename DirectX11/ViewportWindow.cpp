#include "ViewportWindow.h"

#include "EditorWorld.h"
#include "GameWorld.h"

#include "EditorActor.h"
#include "EditorCamera.h"

#include "AssetManager.h"
#include "IAssetFile.h"

#include "Map.h"
#include "GlobalVariable.h"

#include "IIntersectable.h"
#include "EditorWorld.h"

using namespace std;

ViewportWindow::ViewportWindow(EditorWorld* EditorWorldIn)
    : IEditorLinkedWindow(EditorWorldIn)
{
    EditorActor* EditorActorInstance= EditorWorldCached->GetEditorActorInstance();
    assert(EditorActorInstance != nullptr);

    EditorCameraCached = EditorActorInstance->GetEditorCameraCached();
    assert(EditorCameraCached != nullptr);
}

ViewportWindow::~ViewportWindow()
{
}

void ViewportWindow::RenderWindow()
{
    ImGui::Begin("Viewport");
    Map* CurrentMap = GameWorldCached->GetCurrentMap();
    if (CurrentMap != nullptr)
    {
        if (EditorCameraCached != nullptr)
        {
            ImagePosition = ImGui::GetCursorScreenPos();
            ImageSize = ImGui::GetContentRegionAvail();
            ImGui::Image(EditorCameraCached->GetResolvedSceneSRV(), ImageSize);
            ManageAssetDrop(CurrentMap);
            ManageMouseLBClick(CurrentMap);
        }
    }
    ImGui::End();
}

void ViewportWindow::ManageAssetDrop(Map* CurrentMap)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragDrop::GAsset))
        {
            IM_ASSERT(payload->DataSize == sizeof(IAssetFile*));
            IAssetFile* AssetFile = *(IAssetFile**)(payload->Data);

            if (CurrentMap != nullptr)
            {
                ImGuiIO& io = ImGui::GetIO();
                ImVec2 AbsMousePos = io.MousePos;
                ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - ImagePosition.x, AbsMousePos.y - ImagePosition.y);
                switch (AssetFile->GetAssetType())
                {
                case EAssetType::StaticMesh:
                case EAssetType::SkeletalMesh:
                {
                    Ray ClickedRay = Ray::CreateRay(
                        RelativeMousePos.x, RelativeMousePos.y, 
                        ImageSize.x, ImageSize.y,
                        EditorCameraCached->GetProjectionMatrix(), 
                        EditorCameraCached->GetViewMatrix()
                    );
                    const XMVECTOR PlacePositon = ClickedRay.Origin + ClickedRay.Direction * (500.f);

                    CurrentMap->AddRenderObject(
                        (AMeshAsset*)AssetFile,
                        PlacePositon.m128_f32[0],
                        PlacePositon.m128_f32[1],
                        PlacePositon.m128_f32[2]
                    );                            
                    break;
                }
                default:
                    break;
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void ViewportWindow::ManageMouseLBClick(Map* CurrentMap)
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 AbsMousePos = io.MousePos;
        ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - ImagePosition.x, AbsMousePos.y - ImagePosition.y);

        UINT SelectedID = EditorCameraCached->GetID(RelativeMousePos.x, RelativeMousePos.y, ImageSize.x, ImageSize.y);
        EditorWorldCached->SetSelecteObjectByID(SelectedID);
    }
}
