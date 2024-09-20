#pragma once
#include <Windows.h>

class IWorld
{
public:
	virtual void RenderWorld() = 0;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

