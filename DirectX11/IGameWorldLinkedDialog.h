#pragma once
#include "IDialog.h"

class GameWorld;

class IGameWorldLinkedDialog : public IDialog
{
public:
	IGameWorldLinkedDialog(GameWorld* GameWorldLinkedIn);
	virtual ~IGameWorldLinkedDialog();

protected:
	GameWorld* GameWorldLinked = nullptr;
};

