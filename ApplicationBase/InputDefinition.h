#pragma once
#include <Windows.h>

enum class EMouseAction
{
	MOUSE_MOVE,
	MOUSE_LB_BUTTONDOWN,
	MOUSE_LB_BUTTONPRESSED,
	MOUSE_LB_BUTTONUP,
	MOUSE_RB_BUTTONDOWN,
	MOUSE_RB_BUTTONPRESSED,
	MOUSE_RB_BUTTONUP,
	MOUSE_MB_BUTTONDOWN,
	MOUSE_MB_BUTTONPRESSED,
	MOUSE_MB_BUTTONUP,
};

enum class EMouseInput
{
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,
};

#define MouseActionCount static_cast<size_t>(EMouseAction::MOUSE_MB_BUTTONUP) + 1
#define MouseInputCount static_cast<size_t>(EMouseInput::MOUSE_RIGHT) + 1

enum class EKeyAction
{
	KEY_DOWN,
	KEY_PRESSED,
	KEY_UP
};

enum class EKeyInput
{
	KEY_NONE,
	KEY_SHIFT,
	KEY_CTRL,
	KEY_ALT,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z
};

#define KeyActionCount static_cast<size_t>(EKeyAction::KEY_UP) + 1
#define KeyInputCount static_cast<size_t>(EKeyInput::KEY_Z) + 1

