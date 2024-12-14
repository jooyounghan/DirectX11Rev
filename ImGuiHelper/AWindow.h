#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuiControlManager.h"

#include <string>
#include <functional>
class AWindow
{
public:
	AWindow(const std::string& windowID);

public:
	void ShowWindow();
	virtual void RenderWindowImpl() = 0;

public:
	std::function<void(AWindow*)> OnClose = [&](AWindow* window) {};

protected:
	const std::string m_windowID;
	ImGuiControlManager m_controlManager;
};

