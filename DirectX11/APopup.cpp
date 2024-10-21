#include "APopup.h"

using namespace std;
using namespace ImGui;

APopup::APopup(const std::string& PopupIDIn)
    : PopupID(PopupIDIn)
{
}

APopup::~APopup()
{
}

void APopup::PopUp(const ImVec2& MinRectIn, const ImVec2& MaxRectIn)
{
    if (IsMouseHoveringRect(MinRectIn, MaxRectIn))
    {
        if (IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            ImVec2 DragDelta = GetMouseDragDelta(ImGuiMouseButton_Right);
            if (DragDelta.x == 0.0f && DragDelta.y == 0.0f)
            {
                OpenPopup(PopupID.c_str());
            }
        }
    }
    if (BeginPopup(PopupID.c_str()))
    {
        RenderPopupMenu();
        EndPopup();
    }
}
