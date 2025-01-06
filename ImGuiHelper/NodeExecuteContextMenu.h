#pragma once
#include "AContextMenu.h"

class NodeExecuteContextMenu : public AContextMenu
{
public:
	NodeExecuteContextMenu(bool* isEnableAutoPlacement);
	~NodeExecuteContextMenu() override = default;

public:
	std::function<void()> m_onExecuteHandler = [&]() {};
	std::function<void()> m_onToggleHoldHandler = [&]() {};

protected:
	bool* m_isEnableAutoPlacement = nullptr;

protected:
	virtual bool OpenCondition() override;
	virtual void RenderNotification() override;
};

