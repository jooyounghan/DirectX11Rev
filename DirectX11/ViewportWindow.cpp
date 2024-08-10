#include "ViewportWindow.h"

#include "GameWorld.h"
#include "EditorCamera.h"

#include "AssetManager.h"
#include "IAssetFile.h"

#include "Map.h"
#include "GlobalVariable.h"

#include "IIntersectable.h"

using namespace std;

string ViewportWindow::ViewportWindowName = "Viewport";


ViewportWindow::ViewportWindow(GameWorld* GameWorldLinkedIn, EditorCamera* EditorCameraIn)
    : IGameWorldLinkedWindow(GameWorldLinkedIn), EditorCameraCached(EditorCameraIn)
{
}

ViewportWindow::~ViewportWindow()
{
}

void ViewportWindow::RenderWindow()
{
    ImGui::Begin(ViewportWindowName.c_str());
    Map* CurrentMap = GameWorldLinked->GetCurrentMap();
    if (CurrentMap != nullptr)
    {
        if (EditorCameraCached != nullptr)
        {
            ImGui::Image(EditorCameraCached->GetResolvedSceneSRV(), ImGui::GetContentRegionAvail());
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
                ImGuiWindow* ThisWindow = ImGui::FindWindowByName("Viewport");
                if (ThisWindow != nullptr)
                {
                    ImVec2 AbsMousePos = io.MousePos;
                    ImVec2 WinPosition = ThisWindow->Pos;
                    ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - WinPosition.x, AbsMousePos.y - WinPosition.y);
                    switch (AssetFile->GetAssetType())
                    {
                    case EAssetType::StaticMesh:
                    case EAssetType::SkeletalMesh:
                    {
                        Ray ClickedRay = Ray::CreateRay(
                            RelativeMousePos.x, RelativeMousePos.y, 
                            ThisWindow->Size.x, ThisWindow->Size.y, 
                            EditorCameraCached->GetProjectionMatrix(), 
                            EditorCameraCached->GetViewMatrix()
                        );
                        const XMVECTOR PlacePositon = ClickedRay.Origin + ClickedRay.Direction * (500.f);

                        CurrentMap->AddRenderObject(
                            (IMeshAsset*)AssetFile,
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
        }
        ImGui::EndDragDropTarget();
    }
}

void ViewportWindow::ManageMouseLBClick(Map* CurrentMap)
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindow* ThisWindow = ImGui::FindWindowByName("Viewport");
        if (ThisWindow != nullptr)
        {
            //ImVec2 AbsMousePos = io.MousePos;
            //ImVec2 WinPosition = ThisWindow->Pos;
            //ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - WinPosition.x, AbsMousePos.y - WinPosition.y);
            //CurrentMap->Test(RelativeMousePos.x, RelativeMousePos.y, ThisWindow->Size.x, ThisWindow->Size.y);
        }
    }
}
