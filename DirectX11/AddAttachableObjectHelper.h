#pragma once
#include "AAddObjectHelper.h"

class AddAttachableObjectDrawer : public AAddObjectHelper
{
public:
	AddAttachableObjectDrawer(EditorWorld* EditorWorldIn, MapAsset* CurrentMap);

public:
	virtual void DrawAddObjectPopup() override;
};

