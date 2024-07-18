#include "ViewportDialog.h"

#include "GameWorld.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

ViewportDialog::ViewportDialog(GameWorld* GameWorldLinkedIn)
    : IGameWorldLinkedDialog(GameWorldLinkedIn)
{
}

ViewportDialog::~ViewportDialog()
{
}

void ViewportDialog::DoModal()
{
    Camera* GetCurrentGameWorldCamera = GameWorldLinked->GetCurrentCamera();
    
    ImGui::Begin("View Port");
    ImGui::Image(GetCurrentGameWorldCamera->GetSceneSRV(), ImGui::GetContentRegionAvail());
    ImGui::End();
}
