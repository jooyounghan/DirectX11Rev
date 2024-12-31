#pragma once
#include "imgui.h"
#include <string>
#include <functional>

class ANotificator
{
public:
	ANotificator(const std::string& notificatorName);

protected:
	const std::string m_notificatorName;

public:
	virtual void DrawNotificator();

public:
	std::function<bool(const char*, ImGuiWindowFlags_)> m_notificatorOpener;

protected:
	virtual bool OpenCondition() = 0;
	virtual bool CloseCondition() = 0;
	virtual void RenderNotification() = 0;
};

