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

void TaskModal::SetTaskCount(const size_t& taskLeftCount)
{
	m_taskLeftCount = taskLeftCount;
	m_currentTaskDescription = std::format("{}({} Left)", m_taskDescription, to_string(m_taskLeftCount));
}

void TaskModal::SetTaskDescription(const size_t& taskLeftCount, const std::string& taskDescription)
{
	m_isTaskProcessing = true;
	m_taskDescription = taskDescription;
	SetTaskCount(taskLeftCount);
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
