#include "MouseClickEventArgs.h"


MouseClickEventArgs::MouseClickEventArgs(
	const float& mousePosX, const float& mousePosY,
	const float& mouseDeltaPosX, const float& mouseDeltaPosY,
	const EMouseSource& mouseSource, const EMouseEvent& mouseEvent,
	const float& downTime
)
	: MouseEventArgs(mousePosX, mousePosY, mouseDeltaPosX, mouseDeltaPosY)
{
	m_mouseSource = mouseSource;
	m_mouseEvent = mouseEvent;
	m_downTime = downTime;
}

MouseClickEventArgs::MouseClickEventArgs(const MouseEventArgs& mouseEventArgs, const EMouseSource& mouseSource, const EMouseEvent& mouseEvent, const float& downTime)
	: MouseEventArgs(mouseEventArgs)
{
	m_mouseSource = mouseSource;
	m_mouseEvent = mouseEvent;
	m_downTime = downTime;
}

MouseClickEventArgs::MouseClickEventArgs(MouseEventArgs&& mouseEventArgs, const EMouseSource& mouseSource, const EMouseEvent& mouseEvent, const float& downTime)
	: MouseEventArgs(mouseEventArgs)
{
	m_mouseSource = mouseSource;
	m_mouseEvent = mouseEvent;
	m_downTime = downTime;
}
