#pragma once
#include "IRenderable.h"

#include <Windows.h>

class IWorld
{
public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

