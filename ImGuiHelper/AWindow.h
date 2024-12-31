#pragma once
#include "ImGuiControlManager.h"

class AWindow : public ImGuiControlManager
{
public:
	AWindow(const std::string& windowID);

public:
	void ShowWindow();
	virtual void PrepareWindow() = 0;

private:
	virtual void RenderWindowImpl() = 0;

public:
	std::function<void(AWindow*)> OnClose = [&](AWindow* window) {};

protected:
	const std::string m_windowID;
};

