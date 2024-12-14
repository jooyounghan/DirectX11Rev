#include "AWindow.h"

using namespace ImGui;

AWindow::AWindow(const std::string& windowID)
	: m_windowID(windowID)
{
}

void AWindow::ShowWindow()
{
	m_controlManager.CheckMouseControlEvents();

	ImGui::Begin(m_windowID.c_str());
	RenderWindowImpl();
	ImGui::End();
}
