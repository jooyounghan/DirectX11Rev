#include "AddPlaceableObjectHelper.h"

using namespace ImGui;

AddPlaceableObjectDrawer::AddPlaceableObjectDrawer(
    EditorWorld* EditorWorldIn, 
    Map* CurrentMap
)
    : AAddObjectHelper(EditorWorldIn, CurrentMap)
{
}

void AddPlaceableObjectDrawer::DrawAddObjectPopup()
{






    Text("Test1");
    Separator();
    if (Button("OK", ImVec2(120, 0))) { CloseCurrentPopup(); }
    SetItemDefaultFocus();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0))) { CloseCurrentPopup(); }
}
