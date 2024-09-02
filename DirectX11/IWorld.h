#pragma once
#include <Windows.h>
#include "IUpdatable.h"

class IWorld : public IUpdatable
{
public:
	virtual void RenderWorld() = 0;

public:
	virtual void Update(const float& DeltaTimeIn) = 0;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

