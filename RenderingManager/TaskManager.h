#pragma once
#include <functional>
#include <queue>
#include <thread>
#include <string>

class TaskManager
{
	typedef std::function<void()> Task;

public:
	TaskManager();
	~TaskManager();

protected:
	std::queue<std::pair<Task, std::string>> m_serialTasks;
	std::thread m_taskThread;

public:
	std::function<void(const size_t&)> OnTasksLaunched = [&](const size_t&) { };
	std::function<void(const std::string&)> OnTaskStarted = [&](const std::string&) {};
	std::function<void()> OnTasksCompleted = [&]() {};


public:
	void RegisterTask(const Task& task, const std::string& decription);
	void LaunchTasks();
};

