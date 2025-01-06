#include "ImGuiListBox.h"

using namespace std;
using namespace ImGui;

ImGuiListBox::ImGuiListBox(const string& id, const ImVec2& size)
	: AImGuiSelectable(id), m_size(size)
{
}

void ImGuiListBox::Draw()
{
    if (m_selectableItems.size() > 0)
    {
        if (BeginListBox(m_id.c_str(), m_size))
        {
            for (size_t idx = 0; idx < m_selectableItems.size(); ++idx)
            {
                const bool is_selected = (m_selectedIdx == idx);
                if (Selectable(m_selectableItems[idx].c_str(), is_selected))
                {
                    m_selectedIdx = idx;
                    OnSelChanged(m_selectedIdx, m_selectableItems[m_selectedIdx]);
                }

                if (is_selected)
                    SetItemDefaultFocus();
            }
            EndListBox();
        }
    }
}
