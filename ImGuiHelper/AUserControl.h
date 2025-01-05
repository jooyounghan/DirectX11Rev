#pragma once
#include "AInteractable.h"

class AUserControl : public AInteractable
{
public:
	~AUserControl() override = default;

protected:
	ImVec2 m_leftTop;

protected:
	void* m_dragSource = nullptr;

public:
	void RenderControl();

protected:
	virtual void RenderControlImpl() = 0;
	virtual void AdjustPosition();
};

