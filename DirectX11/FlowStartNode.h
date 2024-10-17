#pragma once
#include "FlowNode.h"

class FlowStartNode : public FlowNode
{
public:
	FlowStartNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn
	);
	virtual ~FlowStartNode();
};

