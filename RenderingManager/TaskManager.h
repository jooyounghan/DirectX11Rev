#pragma once
#include <functional>
#include <queue>
#include <thread>
#include <string>
#include <shared_mutex>

#define ON_MAIN_THREAD
#define ON_LAUNCHED_THREAD

class TaskManager
{
	typedef std::function<void()> Task;

	struct SSerialTask
	{
		Task task;
		std::string description;
	};

private:
	TaskManager() = default;
	TaskManager(const TaskManager& engine) = delete;
	TaskManager(TaskManager&& engine) = delete;

public:
	static TaskManager* GetInstance();

protected:
	std::queue<SSerialTask> m_serialTasks;
	std::thread m_backgroundThread;
	std::shared_mutex m_taskMutex;

protected:
	bool m_isLauncingTasks = false;
	bool m_isTaskProcessing = false;

public:
	ON_MAIN_THREAD std::function<void(const size_t&)> OnTaskInserted = [&](const size_t&) {};
	ON_MAIN_THREAD std::function<void(const size_t&, const std::string&)> OnTaskStarted = [&](const size_t&, const std::string&) {};
	ON_MAIN_THREAD std::function<void()> OnTasksCompleted = [&]() {};

public:
	void ON_LAUNCHED_THREAD RegisterTask(const Task& task, const std::string& decription);

public:
	void ON_MAIN_THREAD StartLaunchingTasks();
	void ON_MAIN_THREAD FinishLaunchingTasks();
};

