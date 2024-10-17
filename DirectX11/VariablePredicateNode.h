#pragma once
#include "VariableNode.h"

class VariablePredicateNode : public VariableNode
{
public:
	VariablePredicateNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const size_t InputVariableCountIn
	);
	virtual ~VariablePredicateNode();
};

