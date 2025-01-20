#include "AWindow.h"

using namespace ImGui;

AWindow::AWindow(const std::string& windowID, const bool& isAlwaysOpen, bool* openFlag, ImGuiWindowFlags_ windowFlag)
	: m_windowID(windowID), m_isAlwaysOpen(isAlwaysOpen), m_openFlag(openFlag), m_windowFlag(windowFlag)
{
}

void AWindow::ShowWindow()
{
	if (!m_isAlwaysOpen && !*m_openFlag) return;

	ImGui::Begin(m_windowID.c_str(), m_isAlwaysOpen ? NULL : m_openFlag, m_windowFlag);
	RenderWindowImpl();

	if (IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows))
	{
		CheckMouseControlEvents();
	}
	ImGui::End();



}
