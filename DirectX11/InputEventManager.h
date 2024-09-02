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

protected:
	bool IsKeyPressed[KeyInputCount];

protected:
	MouseEvent MouseEvent[MouseActionCount]

protected:
	static std::unordered_map<WPARAM, EKeyInput> WinMsgToKeyInput;

public:
	KeyEvent& GetKeyEvent(EKeyAction KeyActionEnum, EKeyInput KeyInputEnum);
	virtual void Update(const float& DeltaTimeIn) override;

public:
	void ManageInput(UINT msg, WPARAM wParam, LPARAM lParam);
};

