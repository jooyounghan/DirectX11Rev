#include "AImGuiSelectable.h"

AImGuiSelectable::AImGuiSelectable(
	const std::vector<std::string>& items,
	const std::string& id
)
	: m_id(id), m_items(items)
{
}
