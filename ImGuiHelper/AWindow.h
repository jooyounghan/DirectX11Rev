#pragma once
#include "InteractionManager.h"
#include "imgui.h"

#include <string>

class AWindow : public InteractionManager
{
public:
	AWindow(const std::string& windowID, const bool& isAlwaysOpen, bool* openFlag);
	~AWindow() override = default;

public:
	void ShowWindow();
	virtual void PrepareWindow() = 0;

protected:
	virtual void RenderWindowImpl() = 0;

public:
	std::function<void(AWindow*)> OnClose = [&](AWindow* window) {};

protected:
	const std::string m_windowID;
	const bool m_isAlwaysOpen;
	bool* m_openFlag;
};

