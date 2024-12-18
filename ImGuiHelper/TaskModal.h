#pragma once
#include "AModal.h"

class TaskModal : public AModal
{
public:
	TaskModal(const std::string& modalHeaderName, const std::string& description);
	virtual ~TaskModal();

protected:
	bool m_isTaskProcessing;
	std::string m_description;

protected:
	virtual bool ModalCondition() override final;
	virtual void RenderModal() override final;
};

