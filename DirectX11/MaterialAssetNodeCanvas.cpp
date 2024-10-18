#include "MaterialAssetNodeCanvas.h"
#include "MaterialAsset.h"
#include "VariableInputNode.h"
#include "VariableOutputNode.h"

using namespace std;
using namespace ImGui;


MaterialAssetNodeCanvas::MaterialAssetNodeCanvas(AssetManager* AssetMangerIn, AAssetFile* AssetFileIn)
    : AAssetNodeCanvas(AssetMangerIn, AssetFileIn), MaterialAssetCached(dynamic_cast<MaterialAsset*>(AssetFileIn))
{
    AddNodeElement<VariableInputNode>(ImVec2(0.f, 0.f), ImVec2(100.f, 100.f));
}

MaterialAssetNodeCanvas::~MaterialAssetNodeCanvas()
{
}

void MaterialAssetNodeCanvas::ShowContextMenu()
{
    ImVec2 DragDelta = GetMouseDragDelta(ImGuiMouseButton_Right);
    if (DragDelta.x == 0.0f && DragDelta.y == 0.0f)
    {
        OpenPopupOnItemClick("CavnasContextMenu", ImGuiPopupFlags_MouseButtonRight);
    }
    if (BeginPopup("CavnasContextMenu"))
    {
        if (MenuItem("Add Node", NULL, false, true)) {  }
        if (MenuItem("Add", NULL, false, true)) {  }
        EndPopup();
    }
}
