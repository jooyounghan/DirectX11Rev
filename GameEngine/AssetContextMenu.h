#pragma once
#include "AContextMenu.h"

class AssetContextMenu : public AContextMenu
{
public:
	AssetContextMenu(const std::string& modalHeaderName);
	~AssetContextMenu();

protected:
	virtual bool OpenCondition() override;
	virtual void RenderNotification() override;
};

