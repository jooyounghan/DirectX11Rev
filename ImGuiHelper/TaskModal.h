#pragma once
#include "AModal.h"

class TaskModal : public AModal
{
public:
	TaskModal(const std::string& modalHeaderName);
	virtual ~TaskModal();

protected:
	bool m_isTaskProcessing;
	std::string m_currentTaskDescription;
	size_t m_taskTotalCount = 0;
	size_t m_taskDoneCount = 0;
	float m_taskDonePercent = 0.f;

public:
	void SetTasksLaunched(const size_t& taskTotalCount);
	void SetTaskDescription(const std::string& taskDescription);
	void SetTasksCompleted();

protected:
	virtual bool ModalCondition() override final;
	virtual void RenderModal() override final;
};

