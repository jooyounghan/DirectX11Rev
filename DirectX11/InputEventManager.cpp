#include "InputEventManager.h"

using namespace std;

unordered_map<WPARAM, EKeyInput> InputEventManager::WinMsgToKeyInput
{
	{ 0x30, EKeyInput::KEY_0 },
	{ 0x31, EKeyInput::KEY_1 },
	{ 0x32, EKeyInput::KEY_2 },
	{ 0x33, EKeyInput::KEY_3 },
	{ 0x34, EKeyInput::KEY_4 },
	{ 0x35, EKeyInput::KEY_5 },
	{ 0x36, EKeyInput::KEY_6 },
	{ 0x37, EKeyInput::KEY_7 },
	{ 0x38, EKeyInput::KEY_8 },
	{ 0x39, EKeyInput::KEY_9 },
	{ 0x41, EKeyInput::KEY_A },
	{ 0x42, EKeyInput::KEY_B },
	{ 0x43, EKeyInput::KEY_C },
	{ 0x44, EKeyInput::KEY_D },
	{ 0x45, EKeyInput::KEY_E },
	{ 0x46, EKeyInput::KEY_F },
	{ 0x47, EKeyInput::KEY_G },
	{ 0x48, EKeyInput::KEY_H },
	{ 0x49, EKeyInput::KEY_I },
	{ 0x4A, EKeyInput::KEY_J },
	{ 0x4B, EKeyInput::KEY_K },
	{ 0x4C, EKeyInput::KEY_L },
	{ 0x4D, EKeyInput::KEY_M },
	{ 0x4E, EKeyInput::KEY_N },
	{ 0x4F, EKeyInput::KEY_O },
	{ 0x50, EKeyInput::KEY_P },
	{ 0x51, EKeyInput::KEY_Q },
	{ 0x52, EKeyInput::KEY_R },
	{ 0x53,EKeyInput::KEY_S },
	{ 0x54,EKeyInput::KEY_T },
	{ 0x55,EKeyInput::KEY_U },
	{ 0x56,EKeyInput::KEY_V },
	{ 0x57,EKeyInput::KEY_W },
	{ 0x58,EKeyInput::KEY_X },
	{ 0x59,EKeyInput::KEY_Y },
	{ 0x5A,EKeyInput::KEY_Z }
};

KeyEvent& InputEventManager::GetKeyEvent(EKeyAction KeyActionEnum, EKeyInput KeyInputEnum)
{
	const size_t KeyAction = static_cast<size_t>(KeyActionEnum);
	const size_t KeyInput = static_cast<size_t>(KeyInputEnum);

	return KeyEvents[KeyAction][KeyInput];
}

void InputEventManager::Update(const float& DeltaTimeIn)
{
	const size_t KeyPressed = static_cast<size_t>(EKeyAction::KEY_PRESSED);
	for (size_t idx = 0; idx < KeyInputCount; ++idx)
	{
		if (IsKeyPressed[idx])
		{
			KeyEvents[KeyPressed][idx].RunEventHandlers(DeltaTimeIn);
		}
	}
}

void InputEventManager::ManageInput(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		const size_t KeyDown = static_cast<size_t>(EKeyAction::KEY_DOWN);
		const size_t KeyInput = static_cast<size_t>(WinMsgToKeyInput[wParam]);
		KeyEvents[KeyDown][KeyInput].RunEventHandlers(NULL);
		IsKeyPressed[KeyInput] = true;
		break;
	}
	case WM_KEYUP:
	{
		const size_t KeyUp = static_cast<size_t>(EKeyAction::KEY_UP);
		const size_t KeyInput = static_cast<size_t>(WinMsgToKeyInput[wParam]);
		KeyEvents[KeyUp][KeyInput].RunEventHandlers(NULL);
		IsKeyPressed[KeyInput] = false;
		break;
	}
	default:
		break;
	}
}