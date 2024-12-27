#include "TaskManager.h"
#include "D3D11Engine.h"
#include <d3d11.h>

using namespace std;

TaskManager* TaskManager::GetInstance()
{
	static TaskManager taskManager;
	return &taskManager;
}

void TaskManager::RegisterTask(const Task& task, const std::string& decription)
{
	SSerialTask serialTask;
	serialTask.task = task;
	serialTask.description = decription;
	{
		unique_lock<shared_mutex> writeLock(m_taskMutex);

		m_serialTasks.push(move(serialTask));
		OnTaskInserted(m_serialTasks.size());
	}
}

void ON_MAIN_THREAD TaskManager::StartLaunchingTasks()
{
	m_isLauncingTasks = true;
	m_backgroundThread = thread([&]()
		{
			while (m_isLauncingTasks)
			{
				SSerialTask serialTask;
				{
					unique_lock<shared_mutex> writeLock(m_taskMutex);
					if (!m_serialTasks.empty())
					{
						m_isTaskProcessing = true;
						serialTask = m_serialTasks.front();
						m_serialTasks.pop();
						OnTaskStarted(m_serialTasks.size(), serialTask.description);
					}
					else
					{
						if (m_isTaskProcessing)
						{
							m_isTaskProcessing = false;
							OnTasksCompleted();
						}
					}
				}

				if (m_isTaskProcessing) serialTask.task();

				this_thread::sleep_for(chrono::milliseconds(100));
			}
		}
	);

}

void ON_MAIN_THREAD TaskManager::FinishLaunchingTasks()
{
	m_isLauncingTasks = false;
}
