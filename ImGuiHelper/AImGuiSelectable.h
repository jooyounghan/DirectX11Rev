#pragma once
#include "imgui.h"

#include <string>
#include <vector>
#include <functional>

class AImGuiSelectable
{
public:
	AImGuiSelectable(
		const std::vector<std::string>& items,
		const std::string& id
	);

protected:
	const std::string m_id;
	const std::vector<std::string> m_items;

protected:
	size_t m_selectedIdx = 0;

public:
	std::function<void(const size_t&, const std::string&)> OnSelChanged = [&](const size_t&, const std::string&) {};

public:
	virtual void Draw() = 0;
};

