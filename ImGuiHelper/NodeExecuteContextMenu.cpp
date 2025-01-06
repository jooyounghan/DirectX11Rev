#include "NodeExecuteContextMenu.h"
#include <exception>

using namespace std;
using namespace ImGui;

NodeExecuteContextMenu::NodeExecuteContextMenu(bool* isEnableAutoPlacement)
    : AContextMenu("Execute Nodes"), m_isEnableAutoPlacement(isEnableAutoPlacement)
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
    bool& isEnableAutoPlacement = *m_isEnableAutoPlacement;
    if (MenuItem("Execute"))
    {
        try
        {
            m_onExecuteHandler();
        }
        catch (const exception& e)
        {
            // 안될 경우!
            bool test = true;
        }
    }
    if (MenuItem(isEnableAutoPlacement ? "Disenable Auto Placement" : "Enable Auto Placement"))
    {
        isEnableAutoPlacement = !(isEnableAutoPlacement);
    }
}
