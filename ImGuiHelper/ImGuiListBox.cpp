#include "ImGuiListBox.h"

using namespace std;
using namespace ImGui;

ImGuiListBox::ImGuiListBox(const string& id, const ImVec2& size)
	: AImGuiSelectable(id), m_size(size)
{
}

bool ImGuiListBox::Draw()
{
    bool result = false;
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
                    result = true;
                }

                if (is_selected)
                    SetItemDefaultFocus();
            }
            EndListBox();
        }
    }
    return result;
}
