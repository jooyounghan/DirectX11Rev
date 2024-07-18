#pragma once
#include "IGameWorldLinkedDialog.h"

class ViewportDialog : public IGameWorldLinkedDialog
{
public: 
	ViewportDialog(GameWorld* GameWorldLinkedIn);
	virtual ~ViewportDialog();


public:
	virtual void DoModal() override;
};

