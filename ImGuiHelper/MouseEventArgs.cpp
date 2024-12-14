#include "MouseEventArgs.h"

MouseEventArgs::MouseEventArgs(const float& mousePosX, const float& mousePosY, const float& mouseDeltaPosX, const float& mouseDeltaPosY)
{
	m_mousePosX = mousePosX;
	m_mousePosY = mousePosY;
	m_mousePosDeltaX = mouseDeltaPosX;
	m_mousePosDeltaY = mouseDeltaPosY;
}
