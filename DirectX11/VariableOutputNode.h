#pragma once
#include "VariableNode.h"

class VariableOutputNode : public VariableNode
{
public:
	VariableOutputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn
	);
	virtual ~VariableOutputNode();
};

