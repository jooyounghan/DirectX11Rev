#pragma once
#include "APopup.h"

typedef std::function<void()> BaseTextureAddedHandler;

class AddBaseTexturePopup : public APopup
{
public:
	AddBaseTexturePopup(const std::string& PopupIDIn);
	virtual ~AddBaseTexturePopup();

protected:
	virtual void RenderPopupMenu() override;

public:
	Delegation<> BaseTextureAdded;
};
