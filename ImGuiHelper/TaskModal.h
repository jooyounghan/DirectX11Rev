#pragma once
#include "AModal.h"
#include <shared_mutex>

class TaskModal : public AModal
{
public:
	TaskModal(const std::string& modalHeaderName);
	~TaskModal() override = default;

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
	virtual bool OpenCondition() override final;
	virtual bool CloseCondition() override final;
	virtual void RenderNotification() override final;
};

