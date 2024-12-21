#include "AModal.h"

using namespace std;
using namespace ImGui;

AModal::AModal(const std::string& modalHeaderName)
    : m_modalHeaderName(modalHeaderName)
{
}

AModal::~AModal()
{
}

void AModal::DoModal()
{
    bool isModalAllow = ModalCondition();
    if (isModalAllow)
    {
        OpenPopup(m_modalHeaderName.c_str());
    }

    SetWindowPosToCenter();
    if (BeginPopupModal(m_modalHeaderName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        RenderModal();
        if (!isModalAllow)
        {
            CloseCurrentPopup();
        }
        EndPopup();
    }
    ResetWindowPosToPrevious();
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
