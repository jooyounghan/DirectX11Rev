#include "AUserControl.h"

using namespace ImGui;

void AUserControl::RenderControl()
{
	const ImVec2 cursorScrennPos = GetCursorScreenPos();
	m_left = cursorScrennPos.x;
	m_top = cursorScrennPos.y;

	RenderControlImpl();
}
