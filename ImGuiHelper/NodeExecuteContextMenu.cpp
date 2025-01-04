#include "NodeExecuteContextMenu.h"

using namespace ImGui;

NodeExecuteContextMenu::NodeExecuteContextMenu()
    : AContextMenu("Execute Nodes")
{
}

bool NodeExecuteContextMenu::OpenCondition()
{
    const ImVec2 minRect = GetWindowPos();
    const ImVec2 rectSize = GetWindowSize();
    const ImVec2 maxRect = ImVec2(minRect.x + rectSize.x, minRect.y + rectSize.y);
    return IsMouseHoveringRect(minRect, maxRect) && IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right);

}

void NodeExecuteContextMenu::RenderNotification()
{
    if (MenuItem("Execute"))
    {
        m_onExecuteHandler();
    }
}
