#pragma once
#include "AInteractable.h"

class AUserControl : public AInteractable
{
protected:
	void* m_dragSource = nullptr;

public:
	void RenderControl();

protected:
	virtual void RenderControlImpl() = 0;
	virtual void AdjustPosition() override;
};

