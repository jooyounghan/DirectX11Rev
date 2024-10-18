#pragma once
#include "VariableNode.h"

class VariableOutputNode : public VariableNode
{
public:
	VariableOutputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const size_t& InputVariableCountIn
	);
	virtual ~VariableOutputNode();
};

