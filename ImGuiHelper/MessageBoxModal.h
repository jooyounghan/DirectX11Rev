#pragma once
#include "AModal.h"

class MessageBoxModal : public AModal
{
public:
	MessageBoxModal(const std::string& modalHeaderName);
	virtual ~MessageBoxModal();

protected:
	bool m_isPopped = false;
	std::string m_message;

public:
	void ModalMessage(const std::string& message);

protected:
	virtual bool ModalCondition() override final;
	virtual void RenderModal() override final;
};

