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
	SSerialTask serialTask;
	serialTask.task = task;
	serialTask.description = decription;
	m_serialTasks.push(move(serialTask));
}

void ON_MAIN_THREAD TaskManager::StartLaunchingTasks()
{
	m_isLauncingTasks = true;
	m_backgroundThread = thread([&]()
		{
			while (m_isLauncingTasks)
			{
				size_t taskCounts = m_serialTasks.unsafe_size();
				SSerialTask serialTask;
				if (m_serialTasks.try_pop(serialTask))
				{
					m_isTaskProcessing = true;
					OnTaskStarted(taskCounts, serialTask.description);

					serialTask.task();
					this_thread::sleep_for(chrono::milliseconds(100));
					OnTasksCompleted();
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
		}
	);

}

void ON_MAIN_THREAD TaskManager::FinishLaunchingTasks()
{
	m_isLauncingTasks = false;
}
