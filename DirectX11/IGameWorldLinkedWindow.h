#pragma once
#include "IWindow.h"
#include <string>

class GameWorld;

class IGameWorldLinkedWindow : public IWindow
{
public:
	IGameWorldLinkedWindow(GameWorld* GameWorldLinkedIn);
	virtual ~IGameWorldLinkedWindow();

protected:
	GameWorld* GameWorldLinked = nullptr;
};

