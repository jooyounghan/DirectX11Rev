#pragma once
#include "ANotificator.h"

class AContextMenu : public ANotificator
{
public:
	AContextMenu(const std::string& contextMenuName);

protected:
	virtual bool CloseCondition() override final;
};

