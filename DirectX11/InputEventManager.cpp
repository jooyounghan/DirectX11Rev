#include "InputEventManager.h"
#include <windowsx.h>
#include "GlobalVariable.h"

using namespace std;

unordered_map<WPARAM, EKeyInput> InputEventManager::WinMsgToKeyInput
{
	{ 0x10, EKeyInput::KEY_SHIFT },
	{ 0x11, EKeyInput::KEY_CTRL},
	{ 0x12, EKeyInput::KEY_ALT },
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

EKeyInput InputEventManager::GetKeyInputFromWinMsg(WPARAM wParam)
{
	if (WinMsgToKeyInput.find(wParam) != WinMsgToKeyInput.end())
	{
		return WinMsgToKeyInput[wParam];
	}
	else
	{
		return EKeyInput::KEY_NONE;
	}
}

KeyEvent& InputEventManager::GetKeyEvent(EKeyAction KeyActionEnum, EKeyInput KeyInputEnum)
{
	const size_t KeyAction = static_cast<size_t>(KeyActionEnum);
	const size_t KeyInput = static_cast<size_t>(KeyInputEnum);

	return KeyEvents[KeyAction][KeyInput];
}

MouseEvent& InputEventManager::GetMouseEvnet(EMouseAction MouseActionEnum)
{
	const size_t MouseAction = static_cast<size_t>(MouseActionEnum);
	return MouseEvents[MouseAction];
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
	const size_t KeyValue = static_cast<size_t>(GetKeyInputFromWinMsg(wParam));
	const float NdcX = (GET_X_LPARAM(lParam) - (App::GWidth / 2.f)) / (App::GWidth / 2.f);
	const float NdcY = (GET_Y_LPARAM(lParam) - (App::GHeight / 2.f)) / (App::GHeight / 2.f);

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		const size_t KeyDown = static_cast<size_t>(EKeyAction::KEY_DOWN);
		KeyEvents[KeyDown][KeyValue].RunEventHandlers(NULL);
		IsKeyPressed[KeyValue] = true;
		break;
	}
	case WM_KEYUP:
	{
		const size_t KeyUp = static_cast<size_t>(EKeyAction::KEY_UP);
		KeyEvents[KeyUp][KeyValue].RunEventHandlers(NULL);
		IsKeyPressed[KeyValue] = false;
		break;
	}
	case WM_MOUSEMOVE:
	{
		const size_t MouseMove = static_cast<size_t>(EMouseAction::MOUSE_MOVE);
		MouseEvents[MouseMove].RunEventHandlers(NdcX, NdcY);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		bool test = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		bool test = true;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		bool test = true;
		break;
	}
	case WM_RBUTTONUP:
	{
		bool test = true;
		break;
	}
	case WM_MBUTTONDOWN:
	{
		bool test = true;
		break;
	}
	case WM_MBUTTONUP:
	{
		bool test = true;
		break;
	}

	default:
		break;
	}
}
