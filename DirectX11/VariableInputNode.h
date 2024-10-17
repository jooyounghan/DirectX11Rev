#pragma once
#include "NodeElement.h"

class VariableInputNode : public NodeElement
{
public:
	VariableInputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn
	);
	virtual ~VariableInputNode();
};

