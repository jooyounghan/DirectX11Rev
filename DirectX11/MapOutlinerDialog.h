#pragma once
#include "IGameWorldLinkedDialog.h"

class MapOutlinerDialog : public IGameWorldLinkedDialog
{
public:
	MapOutlinerDialog(GameWorld* GameWorldLinkedIn);
	virtual ~MapOutlinerDialog();

public:
	virtual void DoModal() override;
};

