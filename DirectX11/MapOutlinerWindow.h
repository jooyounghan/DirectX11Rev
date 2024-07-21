#pragma once
#include "IGameWorldLinkedWindow.h"

class MapOutlinerWindow : public IGameWorldLinkedWindow
{
public:
	MapOutlinerWindow(GameWorld* GameWorldLinkedIn);
	virtual ~MapOutlinerWindow();

public:
	virtual void RenderWindow() override;
};

