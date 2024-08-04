#pragma once
#include "IGameWorldLinkedWindow.h"

class Map;

class ViewportWindow : public IGameWorldLinkedWindow
{
public: 
	ViewportWindow(GameWorld* GameWorldLinkedIn);
	virtual ~ViewportWindow();


public:
	virtual void RenderWindow() override;

private:
	static std::string ViewportWindowName;

private:
	void ManageAssetDrop(Map* CurrentMap);
	void ManageMouseLBClick(Map* CurrentMap);
};

