#include "ANotificator.h"

using namespace std;
using namespace ImGui;

ANotificator::ANotificator(const string& notificatorName)
	: m_notificatorName(notificatorName)
{
}

void ANotificator::DrawNotificator()
{
    if (OpenCondition())
    {
        OpenPopup(m_notificatorName.c_str(), ImGuiPopupFlags_::ImGuiPopupFlags_NoOpenOverExistingPopup);
    }

    if (m_notificatorOpener(m_notificatorName.c_str(), ImGuiWindowFlags_AlwaysAutoResize))
    {
        RenderNotification();
        if (CloseCondition())
        {
            CloseCurrentPopup();
        }
        EndPopup();
    }
}
