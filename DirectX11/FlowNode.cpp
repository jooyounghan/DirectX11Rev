#include "FlowNode.h"
#include "GlobalVariable.h"

FlowNode::FlowNode(
	const ImVec2& CenterPositionIn,
	const ImVec2& RectangleSizeIn
)
	: NodeElement(CenterPositionIn, RectangleSizeIn, NodeColor::FlowNodeBaseColor, NodeColor::FlowNodeHilightedColor)
{
}

FlowNode::~FlowNode()
{
}
