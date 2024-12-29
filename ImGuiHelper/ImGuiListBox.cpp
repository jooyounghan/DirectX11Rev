#include "ImGuiListBox.h"

using namespace std;
using namespace ImGui;

ImGuiListBox::ImGuiListBox(
	const vector<string>& items, 
	const string& id, 
	const ImVec2& size
)
	: AImGuiSelectable(items, id), m_size(size)
{
}

void ImGuiListBox::Draw()
{
    if (BeginListBox(m_id.c_str(), m_size))
    {
        for (size_t idx = 0; idx < m_items.size(); ++idx)
        {
            const bool is_selected = (m_selectedIdx == idx);
            if (Selectable(m_items[idx].c_str(), is_selected))
            {
                m_selectedIdx = idx;
                OnSelChanged(m_selectedIdx, m_items[m_selectedIdx]);
            }

            if (is_selected)
                SetItemDefaultFocus();
        }
        EndListBox();
    }

}
