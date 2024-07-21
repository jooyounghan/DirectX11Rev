#include "ViewportWindow.h"

#include "GameWorld.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

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
    ImGui::End();
}
