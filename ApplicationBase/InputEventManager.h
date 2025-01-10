#pragma once
#include "InputDefinition.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

#include <unordered_map>

class InputEventManager
{
public:
	InputEventManager();

protected:
	KeyEvent KeyEvents[KeyActionCount][KeyInputCount];
	bool IsKeyPressed[KeyInputCount];

protected:
	static std::unordered_map<WPARAM, EKeyInput> WinMsgToKeyInput;

protected:
	MouseEvent MouseEvents[MouseActionCount];
	bool IsMousePressed[MouseInputCount];

public:
	static EKeyInput GetKeyInputFromWinMsg(WPARAM wParam);

public:
	void Update(const float& DeltaTimeIn);
	KeyEvent& GetKeyEvent(EKeyAction KeyActionEnum, EKeyInput KeyInputEnum);
	MouseEvent& GetMouseEvent(EMouseAction MouseActionEnum);

public:
	void ManageInputFromWindow(
		const UINT& width, const UINT& height,
		UINT msg, WPARAM wParam, LPARAM lParam
	);
};

