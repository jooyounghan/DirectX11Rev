#pragma once
#include "TaskModal.h"
#include <functional>
#include <unordered_map>
#include <future>

class ImGuiTaskWaiter
{
public:
	ImGuiTaskWaiter() = default;

protected:
	TaskModal m_taskModal;
	std::future<void()> m_task;

public:
	void UpdateWaitStatus();

public:
};

