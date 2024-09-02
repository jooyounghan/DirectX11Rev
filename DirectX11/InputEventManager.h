#pragma once
#include "DefineInput.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "IUpdatable.h"

#include <Windows.h>
#include <unordered_map>

class InputEventManager : public IUpdatable
{
protected:
	KeyEvent KeyEvents[KeyActionCount][KeyInputCount];
	bool IsKeyPressed[KeyInputCount];

protected:
	MouseEvent MouseEvents[MouseActionCount];
	bool IsMousePressed[MouseInputCount];

protected:
	static std::unordered_map<WPARAM, EKeyInput> WinMsgToKeyInput;

public:
	static EKeyInput GetKeyInputFromWinMsg(WPARAM wParam);

public:
	KeyEvent& GetKeyEvent(EKeyAction KeyActionEnum, EKeyInput KeyInputEnum);
	MouseEvent& GetMouseEvnet(EMouseAction MouseActionEnum);

	virtual void Update(const float& DeltaTimeIn) override;

public:
	void ManageInput(UINT msg, WPARAM wParam, LPARAM lParam);
};

