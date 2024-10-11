#pragma once
#include "IWindow.h"
#include "Delegation.h"

#include <string>

class AssetControl;
class AssetControlWindow;

typedef std::function<void(AssetControlWindow*)> AssetControlWindowClosedDelegation;


class AssetControlWindow : public IWindow
{
public:
	AssetControlWindow(AssetControl* AssetControlCachedIn);

protected:
	AssetControl* AssetControlCached = nullptr;
	std::string WindowDescription;

protected:
	ImVec2 WindowSize;

protected:
	bool bIsOpen = true;

public:
	Delegation<AssetControlWindow*> CloseEvent;

public:
	virtual void RenderWindow() override;
};

