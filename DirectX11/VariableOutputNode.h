#pragma once
#include "NodeElement.h"

class VariableOutputNode : public NodeElement
{
public:
	VariableOutputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn
	);
	virtual ~VariableOutputNode();
};

