#include "AUserControl.h"

using namespace ImGui;

void AUserControl::RenderControl()
{
	AdjustPosition();
	RenderControlImpl();
}

void AUserControl::AdjustPosition()
{
	m_leftTop = GetCursorScreenPos();
}
