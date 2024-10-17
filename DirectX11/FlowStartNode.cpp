#include "FlowStartNode.h"
#include "FlowOutputPort.h"

FlowStartNode::FlowStartNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn
)
	: FlowNode(CenterPositionIn, RectangleSizeIn)
{
	AddOutputPort<FlowOutputPort>(
		CenterPositionIn,
		10.f
	);
}

FlowStartNode::~FlowStartNode()
{
}
