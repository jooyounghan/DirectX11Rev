#pragma once
#include "FlowNode.h"

class FlowMidNode : public FlowNode
{
public:
	FlowMidNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const size_t& InputVariablePortCount
	);
	virtual ~FlowMidNode();
};

