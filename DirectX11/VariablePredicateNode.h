#pragma once
#include "NodeElement.h"

class VariablePredicateNode : public NodeElement
{
public:
	VariablePredicateNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const size_t InputVariableCountIn
	);
	virtual ~VariablePredicateNode();
};

