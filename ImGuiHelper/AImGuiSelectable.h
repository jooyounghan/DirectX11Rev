#pragma once
#include "imgui.h"

#include <string>
#include <vector>
#include <functional>

constexpr size_t NotSelected = ~0;

class AImGuiSelectable
{
public:
	AImGuiSelectable(const std::string& id);

protected:
	const std::string m_id;
	std::vector<std::string> m_selectableItems;

public:
	inline void SetSelectableItems(const std::vector<std::string>& selectableItems) { m_selectableItems = selectableItems; }

protected:
	size_t m_selectedIdx = NotSelected;

public:
	std::function<void(const size_t&, const std::string&)> OnSelChanged = [&](const size_t&, const std::string&) {};

public:
	virtual void Draw() = 0;
};

