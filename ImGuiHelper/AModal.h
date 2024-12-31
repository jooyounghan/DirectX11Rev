#pragma once
#include "ANotificator.h"

class AModal : public ANotificator
{
public:
	AModal(const std::string& modalHeaderName);
	~AModal() override = default;

protected:
	ImVec2 m_previousWindowPos = ImVec2();

protected:
	void SetWindowPosToCenter();
	void ResetWindowPosToPrevious();

public:
	virtual void DrawNotificator() override;
};

