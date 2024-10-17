#pragma once
#include "NodeElement.h"

class VariableNode : public NodeElement
{
public:
	VariableNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const ImU32& RectangleColorIn,
		const ImU32& HilightedColorIn
	);

	virtual ~VariableNode();
};

