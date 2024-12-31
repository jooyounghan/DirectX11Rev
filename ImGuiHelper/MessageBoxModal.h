#pragma once
#include "AModal.h"

class MessageBoxModal : public AModal
{
public:
	MessageBoxModal(const std::string& modalHeaderName);
	virtual ~MessageBoxModal();

protected:
	std::string m_message;

public:
	void SetMessage(const std::string& message);

protected:
	virtual bool OpenCondition() override final;
	virtual bool CloseCondition() override final;
	virtual void RenderNotification() override final;
};

