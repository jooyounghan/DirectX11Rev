#pragma once
#include "NodeElement.h"

class FlowNode : public NodeElement
{
public:
	FlowNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn
	);

	virtual ~FlowNode();
};

