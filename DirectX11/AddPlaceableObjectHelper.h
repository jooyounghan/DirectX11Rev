#pragma once
#include "AAddObjectHelper.h"

class AddPlaceableObjectDrawer : public AAddObjectHelper
{
public:
	AddPlaceableObjectDrawer( 
		EditorWorld* EditorWorldIn,
		Map* CurrentMap
	);

public:
	virtual void DrawAddObjectPopup() override;
};

