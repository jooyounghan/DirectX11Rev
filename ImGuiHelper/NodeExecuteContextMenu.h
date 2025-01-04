#pragma once
#include "AContextMenu.h"

class NodeExecuteContextMenu : public AContextMenu
{
public:
	NodeExecuteContextMenu();
	~NodeExecuteContextMenu() override = default;

public:
	std::function<void()> m_onExecuteHandler = [&]() {};

protected:
	virtual bool OpenCondition() override;
	virtual void RenderNotification() override;
};

