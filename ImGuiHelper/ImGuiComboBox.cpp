#include "ImGuiComboBox.h"

using namespace std;
using namespace ImGui;

ImGuiComboBox::ImGuiComboBox(const string& id, const string& label, ImGuiComboFlags flags)
	: AImGuiSelectable(id), m_label(label), m_comboFlags(flags)
{
}

bool ImGuiComboBox::Draw()
{
	bool result = false;
	if (m_selectableItems.size() > 0)
	{
		PushID(m_id.c_str());
		if (BeginCombo(m_label.c_str(), m_selectedIdx == NotSelected ? m_preview.c_str() : m_selectableItems[m_selectedIdx].c_str(), m_comboFlags))
		{
			for (size_t idx = 0; idx < m_selectableItems.size(); idx++)
			{
				const bool isSelected = (idx == m_selectedIdx);
				if (Selectable(m_selectableItems[idx].c_str(), isSelected))
				{
					m_selectedIdx = idx;
					OnSelChanged(m_selectedIdx, m_selectableItems[m_selectedIdx]);
					result = true;
				}

				if (isSelected)
					SetItemDefaultFocus();
			}
			EndCombo();
		}
		PopID();
	}
	else
	{
		Text("None Selectables");
	}
	return result;
}

