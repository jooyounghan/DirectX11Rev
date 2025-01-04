#include "AWindow.h"

using namespace ImGui;

AWindow::AWindow(const std::string& windowID, const bool& isAlwaysOpen, bool* openFlag)
	: m_windowID(windowID), m_isAlwaysOpen(isAlwaysOpen), m_openFlag(openFlag)
{
}

void AWindow::ShowWindow()
{
	if (!m_isAlwaysOpen && !*m_openFlag) return;

	ImGui::Begin(m_windowID.c_str(), m_isAlwaysOpen ? NULL : m_openFlag, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
	RenderWindowImpl();
	ImGui::End();

	CheckMouseControlEvents();
}
