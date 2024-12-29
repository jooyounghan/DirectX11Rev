#include "AImGuiSelectable.h"

class ImGuiListBox : public AImGuiSelectable
{
public:
	ImGuiListBox(
		const std::vector<std::string>& items,
		const std::string& id,
		const ImVec2& size
	);

protected:
	const ImVec2 m_size;

public:
	virtual void Draw() override;
};

