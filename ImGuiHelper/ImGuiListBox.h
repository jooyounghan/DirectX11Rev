#include "AImGuiSelectable.h"

class ImGuiListBox : public AImGuiSelectable
{
public:
	ImGuiListBox(const std::string& id, const ImVec2& size = ImVec2(1.f, 1.f));

protected:
	ImVec2 m_size;

public:
	inline void SetListBoxSize(const ImVec2& size) { m_size = size; }

public:
	virtual bool Draw() override;
};

