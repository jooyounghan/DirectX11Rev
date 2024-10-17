#pragma once
#include "FlowNode.h"
class FlowEndNode : public FlowNode
{
public:
	FlowEndNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn
	);
	virtual ~FlowEndNode();
};

