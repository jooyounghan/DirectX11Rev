#include "MessageBoxModal.h"

MessageBoxModal::MessageBoxModal(const std::string& modalHeaderName)
	: AModal(modalHeaderName)
{
}

MessageBoxModal::~MessageBoxModal()
{
}

void MessageBoxModal::ModalMessage(const std::string& message)
{
	m_isPopped = true;
	m_message = message;
}

bool MessageBoxModal::ModalCondition()
{
	return m_isPopped;
}

void MessageBoxModal::RenderModal()
{
	ImGui::Text(m_message.c_str());
	if (ImGui::Button("OK"))
	{
		m_isPopped = false;
	}
}
