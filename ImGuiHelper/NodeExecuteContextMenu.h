#pragma once
#include "AContextMenu.h"

class NodeExecuteContextMenu : public AContextMenu
{
public:
	NodeExecuteContextMenu(bool* isEnableAutoPlacement);
	~NodeExecuteContextMenu() override;

public:
	std::function<void()> m_onExecuteHandler = [&]() {};

public:
	std::function<void()> m_onExecuteSucceed = [&]() {};
	std::function<void(const std::string&)> m_onExecuteFailed = [&](const std::string&) {};

protected:
	bool* m_isEnableAutoPlacement = nullptr;

protected:
	virtual bool OpenCondition() override;
	virtual void RenderNotification() override;
};

