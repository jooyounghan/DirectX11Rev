#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "Delegation.h"

class AWindow;

typedef std::function<void(AWindow*)> WindowClosedHandler;

class AWindow
{
public:
	virtual void RenderWindow() = 0;

public:
	Delegation<AWindow*> CloseEvent;
};

