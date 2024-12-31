#include "AContextMenu.h"

using namespace std;
using namespace ImGui;

AContextMenu::AContextMenu(const std::string& contextMenuName)
	: ANotificator(contextMenuName)
{
    m_notificatorOpener = [&](const char* id, ImGuiWindowFlags_ flag)
        {
            return BeginPopup(id, flag);
        };
}

bool AContextMenu::CloseCondition()
{
    return false;
}
