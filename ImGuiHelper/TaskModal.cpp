#include "TaskModal.h"
#include <format>

using namespace std;

TaskModal::TaskModal(const string& modalHeaderName)
	: AModal(modalHeaderName), m_isTaskProcessing(false)
{
}

TaskModal::~TaskModal()
{
}

void TaskModal::SetTasksLaunched(const size_t& taskTotalCount)
{
	m_isTaskProcessing = true;
	m_taskTotalCount = taskTotalCount;
	m_taskDoneCount = 0;
}

void TaskModal::SetTaskDescription(const std::string& taskDescription)
{
	m_taskDoneCount++;
	m_taskDonePercent = (m_taskDoneCount / static_cast<float>(m_taskTotalCount)) * 100.f;
	m_currentTaskDescription = std::format("{}({}/{})", taskDescription, to_string(m_taskDoneCount), to_string(m_taskTotalCount));
}

void TaskModal::SetTasksCompleted() 
{ 
	m_isTaskProcessing = false; 
}

bool TaskModal::ModalCondition()
{
	return m_isTaskProcessing;
}

void TaskModal::RenderModal()
{
	ImGui::Text(m_currentTaskDescription.c_str());
}
