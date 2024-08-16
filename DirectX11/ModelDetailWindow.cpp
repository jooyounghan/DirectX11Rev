#include "ModelDetailWindow.h"

#include "ModelDetailedInformationVisitor.h"

#include "EditorWorld.h"
#include "AObject.h"

using namespace std;
using namespace ImGui;

ModelDetailWindow::ModelDetailWindow(EditorWorld* EditorWorldIn)
	: IEditorLinkedWindow(EditorWorldIn)
{
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
    BeginChild("SelectedDetailInformation", ImVec2(RegionAvail.x, RegionAvail.y), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar);

    AObject* SelectedDetailed = EditorWorldCached->GetSelectedDetailed();
    if (SelectedDetailed != nullptr)
    {
        ModelDetailedInformationVisitor InformationVisitor(this);
        SelectedDetailed->AcceptGui(&InformationVisitor);

    }
    EndChild();
}
