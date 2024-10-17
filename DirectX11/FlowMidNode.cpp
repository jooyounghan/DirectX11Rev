#include "FlowMidNode.h"
#include "NodeColorVariable.h"
#include "FlowOutputPort.h"
#include "FlowInputPort.h"

FlowMidNode::FlowMidNode(
	const ImVec2& CenterPositionIn,
	const ImVec2& RectangleSizeIn,
	const size_t& InputVariablePortCount
)
	: FlowNode(CenterPositionIn, RectangleSizeIn)
{
	AddInputPort<FlowInputPort>(CenterPositionIn, 10.f);

	for (size_t idx = 0; idx < InputVariablePortCount; ++idx)
	{
		AddInputPort<FlowInputPort>(CenterPositionIn, 10.f);
	}

	AddOutputPort<FlowOutputPort>(CenterPositionIn, 10.f);
}

FlowMidNode::~FlowMidNode()
{
}
