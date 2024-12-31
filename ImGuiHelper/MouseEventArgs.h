#pragma once

struct MouseEventArgs
{
	MouseEventArgs(
		const float& mousePosX, const float& mousePosY,
		const float& mouseDeltaPosX, const float& mouseDeltaPosY
	);
	float m_mousePosX = 0.f;
	float m_mousePosY = 0.f;
	float m_mousePosDeltaX = 0.f;
	float m_mousePosDeltaY = 0.f;
};

