#include "AModal.h"

using namespace std;
using namespace ImGui;

AModal::AModal(const std::string& modalHeaderName)
    : ANotificator(modalHeaderName)
{
    m_notificatorOpener = [&](const char* id, ImGuiWindowFlags_ flag)
        {
            return BeginPopupModal(id, NULL, flag);
        };
}

void AModal::SetWindowPosToCenter()
{
    m_previousWindowPos = GetWindowPos();
    ImVec2 Center = GetMainViewport()->GetCenter();
    SetWindowPos(Center, ImGuiCond_Appearing);
}

void AModal::ResetWindowPosToPrevious()
{
    SetWindowPos(m_previousWindowPos, ImGuiCond_Appearing);
}

void AModal::DrawNotificator()
{
    SetWindowPosToCenter();
    ANotificator::DrawNotificator();
    ResetWindowPosToPrevious();
}
