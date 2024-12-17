#include "AControl.h"

using namespace ImGui;

void AControl::RenderControl()
{
	const ImVec2 cursorScrennPos = GetCursorScreenPos();
	m_left = cursorScrennPos.x;
	m_top = cursorScrennPos.y;

	RenderControlImpl();
}
