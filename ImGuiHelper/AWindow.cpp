#include "AWindow.h"

using namespace ImGui;

AWindow::AWindow(const std::string& windowID)
	: m_windowID(windowID)
{
}

void AWindow::ShowWindow()
{
	ImGui::Begin(m_windowID.c_str());
	RenderWindowImpl();
	ImGui::End();

	CheckMouseControlEvents();
}
