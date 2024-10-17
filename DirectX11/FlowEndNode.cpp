#include "FlowEndNode.h"
#include "FlowInputPort.h"
#include "NodeColorVariable.h"

FlowEndNode::FlowEndNode(
	const ImVec2& CenterPositionIn,
	const ImVec2& RectangleSizeIn
)
	: FlowNode(CenterPositionIn, RectangleSizeIn)
{
	AddInputPort<FlowInputPort>(CenterPositionIn, 10.f);
}

FlowEndNode::~FlowEndNode()
{
}
