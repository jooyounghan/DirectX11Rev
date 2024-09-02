#pragma once

enum class EKeyAction
{
	KEY_DOWN,
	KEY_PRESSED,
	KEY_UP,
	KEY_ACTION_COUNT
};

#define KeyActionCount static_cast<size_t>(EKeyAction::KEY_ACTION_COUNT)

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
	KEY_Z,
	KEY_INPUT_COUNT
};

#define KeyInputCount static_cast<size_t>(EKeyInput::KEY_INPUT_COUNT)

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
	MOUSE_ACTION_COUNT
};

#define MouseActionCount static_cast<size_t>(EMouseAction::MOUSE_ACTION_COUNT)

enum class EMouseInput
{
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,
	MOUSE_INPUT_COUNT
};

#define MouseInputCount static_cast<size_t>(EMouseInput::MOUSE_INPUT_COUNT)
