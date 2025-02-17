#pragma once
#include "AImGuiSelectable.h"

class ImGuiComboBox : public AImGuiSelectable
{
public:
	ImGuiComboBox(const std::string& id, const std::string& label, ImGuiComboFlags flags = 0);

protected:
	const std::string m_label;
	const ImGuiComboFlags m_comboFlags;

public:
	virtual bool Draw() override;
};

