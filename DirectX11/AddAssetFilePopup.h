#pragma once
#include "APopup.h"

typedef std::function<void()> AssetCreatedHandler;

class AddAssetFilePopup : public APopup
{
public:
	AddAssetFilePopup(const std::string& PopupIDIn);
	virtual ~AddAssetFilePopup();

protected:
	virtual void RenderPopupMenu() override;

public:
	Delegation<> AssetCreated;
};

