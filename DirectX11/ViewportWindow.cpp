#include "ViewportWindow.h"

#include "GraphicsPipeline.h"
#include "GameWorld.h"
#include "Camera.h"
#include "EditorWorld.h"
#include "AssetManager.h"
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
    Camera* GetCurrentGameWorldCamera = GameWorldLinked->GetCurrentCamera();
    
    ImGui::Begin("View Port");
    ImGui::Image(GetCurrentGameWorldCamera->GetSceneSRV(), ImGui::GetContentRegionAvail());

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragDrop::GAsset))
        {
            IM_ASSERT(payload->DataSize == sizeof(IAssetFile*));
            IAssetFile* AssetFile = *(IAssetFile**)(payload->Data);

        }
        ImGui::EndDragDropTarget();
    }
    ImGui::End();
}
