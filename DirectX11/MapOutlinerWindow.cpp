#include "MapOutlinerWindow.h"

#include "ModelListOutlinerVisitor.h"
#include "ModelInformationOutlinerVisitor.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "Map.h"
#include "APlaceable.h"

#include <list> 
#include <memory>

using namespace std;
using namespace ImGui;

MapOutlinerWindow::MapOutlinerWindow(EditorWorld* EditorWorldIn)
	: IEditorLinkedWindow(EditorWorldIn)
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
    Map* CurrentMap = GameWorldCached->GetCurrentMap();
    if (CurrentMap)
    {
        const list<unique_ptr<APlaceable>>& RootPlaceables = CurrentMap->GetRootPlaceables();
        
        ModelListOutlinerVisitor ModelListliner(this);
        for (auto& Placeable : RootPlaceables)
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

    AObject* SelectedObject = EditorWorldCached->GetSelectedObject();
    if (SelectedObject != nullptr)
    {
        ModelInformationOutlinerVisitor InformationVisitor(this);
        SelectedObject->AcceptGui(&InformationVisitor);

    }
    EndChild();
}
