#include "MapOutlinerDialog.h"

MapOutlinerDialog::MapOutlinerDialog(GameWorld* GameWorldLinkedIn)
	: IGameWorldLinkedDialog(GameWorldLinkedIn)
{
}

MapOutlinerDialog::~MapOutlinerDialog()
{
}

void MapOutlinerDialog::DoModal()
{
    ImGui::Begin("Model Outliner");
    ImGui::End();
}
