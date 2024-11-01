#pragma once
#include "APopup.h"

typedef std::function<void()> BaseTextureAddedHandler;
typedef std::function<void()> FloatAddedHandler;
typedef std::function<void()> Float3AddedHandler;

class AddMaterialElementPopup : public APopup
{
public:
	AddMaterialElementPopup(const std::string& PopupIDIn);
	virtual ~AddMaterialElementPopup();

protected:
	virtual void RenderPopupMenu() override;

public:
	Delegation<> BaseTextureAdded;
	Delegation<> FloatAdded;
	Delegation<> Float3Added;
};
