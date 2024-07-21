#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class IWindow
{
public:
	virtual void RenderWindow() = 0;
};

