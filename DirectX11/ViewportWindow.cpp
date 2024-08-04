#include "ViewportWindow.h"

#include "GameWorld.h"
#include "Camera.h"

#include "AssetManager.h"
#include "IAssetFile.h"

#include "Map.h"
#include "GlobalVariable.h"

#include "IIntersectable.h"

using namespace std;

string ViewportWindow::ViewportWindowName = "Viewport";


ViewportWindow::ViewportWindow(GameWorld* GameWorldLinkedIn)
    : IGameWorldLinkedWindow(GameWorldLinkedIn)
{
}

ViewportWindow::~ViewportWindow()
{
}

void ViewportWindow::RenderWindow()
{
    ImGui::Begin(ViewportWindowName.c_str());
    ImGui::Button("ViewportIntersectTester");
    Map* CurrentMap = GameWorldLinked->GetCurrentMap();
    if (CurrentMap != nullptr)
    {
        Camera* CurrentCamera = CurrentMap->GetMapCamera();
        if (CurrentCamera != nullptr)
        {
            ImGui::Image(CurrentCamera->GetSceneSRV(), ImGui::GetContentRegionAvail());
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
                        CurrentMap->AddRenderObject(
                            (IMeshAsset*)AssetFile,
                            RelativeMousePos.x, RelativeMousePos.y,
                            ThisWindow->Size.x, ThisWindow->Size.y
                        );
                        break;
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
            ImVec2 AbsMousePos = io.MousePos;
            ImVec2 WinPosition = ThisWindow->Pos;
            ImVec2 RelativeMousePos = ImVec2(AbsMousePos.x - WinPosition.x, AbsMousePos.y - WinPosition.y);
            CurrentMap->Test(RelativeMousePos.x, RelativeMousePos.y, ThisWindow->Size.x, ThisWindow->Size.y);
        }
    }
}
