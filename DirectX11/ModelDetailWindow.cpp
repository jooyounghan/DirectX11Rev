#include "ModelDetailWindow.h"
#include "ModelDetailedInformationVisitor.h"

#include "EditorWorld.h"
#include "GameWorld.h"
#include "AAttachableObject.h"
#include "APlaceableObject.h"

using namespace std;
using namespace ImGui;

ModelDetailWindow::ModelDetailWindow(EditorWorld* EditorWorldIn)
	: EditorWorldCached(EditorWorldIn)
{
    if (EditorWorldCached != nullptr)
    {
        GameWorld* GameWorldCached = EditorWorldCached->GetGameWorldCached();
        if (GameWorldCached != nullptr)
        {
            AssetManagerCached = GameWorldCached->GetAssetManagerInstance();
        }
    }
}

ModelDetailWindow::~ModelDetailWindow()
{
}

void ModelDetailWindow::RenderWindow()
{

    Begin("Model Detailed");
    DrawDetailProperties();
    End();
}

void ModelDetailWindow::DrawDetailProperties()
{
    ImVec2 RegionAvail = GetContentRegionAvail();
    BeginChild("Selected Detail Information", ImVec2(RegionAvail.x, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);

    APlaceableObject* SelectedPlaceable = EditorWorldCached->GetSelectedPlaceable();
    AAttachableObject* SelectedAttached = EditorWorldCached->GetSelectedAttached();

    ModelDetailedInformationVisitor InformationVisitor(AssetManagerCached);
    if (SelectedAttached != nullptr)
    {
        SelectedAttached->AcceptGui(&InformationVisitor);
    }
    else if (SelectedPlaceable != nullptr)
    {
        SelectedPlaceable->AcceptGui(&InformationVisitor);
    }
    else;

    EndChild();
}
