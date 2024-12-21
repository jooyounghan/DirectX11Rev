#include "TaskManager.h"
#include "D3D11Engine.h"
#include <d3d11.h>

using namespace std;

TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}

void TaskManager::RegisterTask(const Task& task, const std::string& decription)
{
	m_serialTasks.emplace(make_pair(task, decription));
}

void TaskManager::LaunchTasks()
{
	OnTasksLaunched(m_serialTasks.size());

	m_taskThread = thread([&]()
		{
			size_t taskDoneCount = 0;

			queue<pair<Task, string>> serialTasks = move(m_serialTasks);
			m_serialTasks = queue<pair<Task, string>>();

			while (!serialTasks.empty())
			{
				auto serialTask = serialTasks.front();
				serialTasks.pop();

				OnTaskStarted(serialTask.second);
				serialTask.first();

				this_thread::sleep_for(chrono::milliseconds(1000));
			}
			OnTasksCompleted();
		}
	);
}
