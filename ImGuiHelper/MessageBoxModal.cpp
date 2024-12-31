#include "MessageBoxModal.h"

MessageBoxModal::MessageBoxModal(const std::string& modalHeaderName)
	: AModal(modalHeaderName)
{
}

MessageBoxModal::~MessageBoxModal()
{
}

void MessageBoxModal::SetMessage(const std::string& message)
{
	m_message = message;
}

bool MessageBoxModal::OpenCondition()
{
	return !m_message.empty();
}

bool MessageBoxModal::CloseCondition()
{
	return m_message.empty();
}

void MessageBoxModal::RenderNotification()
{
	ImGui::Text(m_message.c_str());
	if (ImGui::Button("OK"))
	{
		m_message.clear();
	}
}
