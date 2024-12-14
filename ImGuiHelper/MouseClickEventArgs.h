#pragma once
#include "MouseEventArgs.h"

enum class EMouseSource
{
	NONE,
	LEFT_BUTTON,
	MIDDLE_BUTTON,
	RIGHT_BUTTON
};

enum class EMouseEvent
{
	NONE,
	CLICKED,
	DBL_CLICKED,
	DOWN,
	RELEASED
};

struct MouseClickEventArgs : MouseEventArgs
{
	MouseClickEventArgs(
		const float& mousePosX, const float& mousePosY,
		const float& mouseDeltaPosX, const float& mouseDeltaPosY,
		const EMouseSource& mouseSource, const EMouseEvent& mouseEvent,
		const float& downTime = 0.f
	);
	MouseClickEventArgs(
		const MouseEventArgs& mouseEventArgs,
		const EMouseSource& mouseSource, const EMouseEvent& mouseEvent,
		const float& downTime = 0.f
	);
	MouseClickEventArgs(
		MouseEventArgs&& mouseEventArgs,
		const EMouseSource& mouseSource, const EMouseEvent& mouseEvent,
		const float& downTime = 0.f
	);
	EMouseSource m_mouseSource;
	EMouseEvent m_mouseEvent;
	float m_downTime;
	bool m_isHandled = false;
};

