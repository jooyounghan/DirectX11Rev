#include "VariableOutputNode.h"
#include "VariableInputPort.h"
#include "NodeColorVariable.h"

VariableOutputNode::VariableOutputNode(const ImVec2& CenterPositionIn, const ImVec2& RectangleSizeIn, const size_t& InputVariableCountIn)
	: VariableNode(
		CenterPositionIn,
		RectangleSizeIn,
		NodeColor::VariableNodeBaseColor,
		NodeColor::VariableNodeHilightedColor
	)
{
	for (size_t idx = 0; idx < InputVariableCountIn; ++idx)
	{
		AddInputPort<VariableInputPort>(ImVec2(NULL, NULL), 10.f);
	}
}

VariableOutputNode::~VariableOutputNode()
{
}
