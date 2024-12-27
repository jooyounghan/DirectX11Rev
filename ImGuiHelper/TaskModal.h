#pragma once
#include "AModal.h"
#include <shared_mutex>

class TaskModal : public AModal
{
public:
	TaskModal(const std::string& modalHeaderName);
	virtual ~TaskModal();

protected:
	bool m_isTaskProcessing;
	std::string m_taskDescription;
	std::string m_currentTaskDescription;
	size_t m_taskLeftCount = 0;

public:
	void SetTaskCount(const size_t& taskLeftCount);
	void SetTaskDescription(const size_t& taskLeftCount, const std::string& taskDescription);
	void SetTasksCompleted();

protected:
	virtual bool ModalCondition() override final;
	virtual void RenderModal() override final;
};

