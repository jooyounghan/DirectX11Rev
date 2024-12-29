#include "ImGuiComboBox.h"

using namespace std;
using namespace ImGui;

ImGuiComboBox::ImGuiComboBox(
	const vector<string>& items, 
	const string& id, 
	const string& label, 
	ImGuiComboFlags flags
)
	: AImGuiSelectable(items, id), m_label(label), m_comboFlags(flags)
{
}

void ImGuiComboBox::Draw()
{
	PushID(m_id.c_str());
	if (BeginCombo(m_label.c_str(), m_items[m_selectedIdx].c_str(), m_comboFlags))
	{
		for (size_t idx = 0; idx < m_items.size(); idx++)
		{
			const bool isSelected = (idx == m_selectedIdx);
			if (Selectable(m_items[idx].c_str(), isSelected))
			{
				m_selectedIdx = idx;
				OnSelChanged(m_selectedIdx, m_items[m_selectedIdx]);
			}

			if (isSelected)
				SetItemDefaultFocus();
		}
		EndCombo();
	}
	PopID();
}

