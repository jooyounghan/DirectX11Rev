#include "MapOutlinerWindow.h"

MapOutlinerWindow::MapOutlinerWindow(GameWorld* GameWorldLinkedIn)
	: IGameWorldLinkedWindow(GameWorldLinkedIn)
{
}

MapOutlinerWindow::~MapOutlinerWindow()
{
}

void MapOutlinerWindow::RenderWindow()
{
    ImGui::Begin("Model Outliner");
    ImGui::End();
}
