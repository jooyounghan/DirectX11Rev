#include "MapOutlinerWindow.h"

#include "ModelListOutlinerVisitor.h"
#include "ModelInformationOutlinerVisitor.h"

#include "GameWorld.h"
#include "Map.h"
#include "APlaceable.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

MapOutlinerWindow::MapOutlinerWindow(GameWorld* GameWorldLinkedIn)
	: IGameWorldLinkedWindow(GameWorldLinkedIn)
{
}

MapOutlinerWindow::~MapOutlinerWindow()
{
}

void MapOutlinerWindow::RenderWindow()
{
    ImGui::Begin("Map Outliner");
    RenderPlaceablesOutline();
    RenderSelectedObjectInformation();
    ImGui::End();
}

void MapOutlinerWindow::RenderPlaceablesOutline()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("PlaceablesOutliner", ImVec2(RegionAvail.x, RegionAvail.y * 0.5f), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    Map* CurrentMap = GameWorldLinked->GetCurrentMap();
    if (CurrentMap)
    {
        const list<unique_ptr<APlaceable>>& Placeables = CurrentMap->GetPlaceables();
        
        ModelListOutlinerVisitor ModelListliner(this);
        for (auto& Placeable : Placeables)
        {
            Placeable->AcceptGui(&ModelListliner);
        }
    }
    EndChild();
}

void MapOutlinerWindow::RenderSelectedObjectInformation()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("SelectedObjectInformation", ImVec2(RegionAvail.x, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);
    if (SelectedObject != nullptr)
    {
        ModelInformationOutlinerVisitor InformationVisitor(this);
        SelectedObject->AcceptGui(&InformationVisitor);

    }
    EndChild();
}
