#include "TaskModal.h"

using namespace std;


TaskModal::TaskModal(const string& modalHeaderName, const string& description)
	: AModal(modalHeaderName), m_isTaskProcessing(false), m_description(description)
{
}

TaskModal::~TaskModal()
{
}

bool TaskModal::ModalCondition()
{
	return m_isTaskProcessing;
}

void TaskModal::RenderModal()
{
	ImGui::Text(m_description.c_str());
}
