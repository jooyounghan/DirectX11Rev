#pragma once
#include "ANotificator.h"

class AContextMenu : public ANotificator
{
public:
	AContextMenu(const std::string& contextMenuName);
	~AContextMenu() override = default;

protected:
	virtual bool CloseCondition() override final;
};

