#include "ViewportWindow.h"

#include "GameWorld.h"
#include "Camera.h"

#include "AssetManager.h"
#include "IAssetFile.h"

#include "Map.h"

#include "GlobalVariable.h"

ViewportWindow::ViewportWindow(GameWorld* GameWorldLinkedIn)
    : IGameWorldLinkedWindow(GameWorldLinkedIn)
{
}

ViewportWindow::~ViewportWindow()
{
}

void ViewportWindow::RenderWindow()
{
    ImGui::Begin("Viewport");

    Map* CurrentMap = GameWorldLinked->GetCurrentMap();
    if (CurrentMap != nullptr)
    {
        Camera* CurrentCamera = CurrentMap->GetMapCamera();
        if (CurrentCamera != nullptr)
        {
            ImGui::Image(CurrentCamera->GetSceneSRV(), ImGui::GetContentRegionAvail());
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
    }
    ImGui::End();
}
