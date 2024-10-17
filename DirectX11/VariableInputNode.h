#pragma once
#include "VariableNode.h"

class VariableInputNode : public VariableNode
{
public:
	VariableInputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn
	);
	virtual ~VariableInputNode();
};

