#include "AssetContextMenu.h"

#include "ANodeEditorWindow.h"

using namespace std;
using namespace ImGui;

AssetContextMenu::AssetContextMenu(const std::string& modalHeaderName)
    : AContextMenu(modalHeaderName)
{
}

AssetContextMenu::~AssetContextMenu()
{
}

bool AssetContextMenu::OpenCondition()
{
    const ImVec2 minRect = GetWindowPos();
    const ImVec2 rectSize = GetWindowSize();
    const ImVec2 maxRect = ImVec2(minRect.x + rectSize.x, minRect.y + rectSize.y);
    return IsMouseHoveringRect(minRect, maxRect) && IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right);
}

void AssetContextMenu::RenderNotification()
{
    static ANodeEditorWindow test("tttttt");
    test.ShowWindow();

    if (BeginMenu("Create Asset"))
    {
        if (MenuItem("Mesh Asset(Static / Skeletal)"))
        {

        }
        if (MenuItem("Texture Asset(Base / Scratch)"))
        {

        }
        if (MenuItem("Model Material"))
        {

        }
        if (MenuItem("IBL Material"))
        {

        }
        if (MenuItem("Animtion"))
        {

        }
        if (MenuItem("Retargeted Animation"))
        {

        }
        ImGui::EndMenu();
    }
    if (BeginMenu("Modify Asset"))
    {
        if (MenuItem("Model Material"))
        {

        }
        if (MenuItem("IBL Material"))
        {

        }
        ImGui::EndMenu();
    }
}

