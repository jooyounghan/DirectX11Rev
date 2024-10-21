#include "AModal.h"

#include <format>

using namespace std;
using namespace ImGui;

AModal::AModal(const std::string& ModalHeaderNameIn)
    : ModalHeaderName(ModalHeaderNameIn)
{
}

AModal::~AModal()
{
}

void AModal::DoModal()
{
	if (ModalCondition())
	{
		OpenPopup(ModalHeaderName.c_str());
	}

    SetWindowPosToCenter();
    if (BeginPopupModal(ModalHeaderName.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        RenderModal();
        EndPopup();
    }
    ResetWindowPosToPrevious();
}

void AModal::SetWindowPosToCenter()
{
    PreviousWindowPos = GetWindowPos();
    ImVec2 Center = GetMainViewport()->GetCenter();
    SetWindowPos(Center, ImGuiCond_Appearing);
}

void AModal::ResetWindowPosToPrevious()
{
    SetWindowPos(PreviousWindowPos, ImGuiCond_Appearing);
}
