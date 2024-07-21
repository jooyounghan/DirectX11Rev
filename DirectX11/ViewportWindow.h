#pragma once
#include "IGameWorldLinkedWindow.h"

class ViewportWindow : public IGameWorldLinkedWindow
{
public: 
	ViewportWindow(GameWorld* GameWorldLinkedIn);
	virtual ~ViewportWindow();


public:
	virtual void RenderWindow() override;
};

