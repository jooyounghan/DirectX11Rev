#include "VariableOutputNode.h"
#include "VariableInputPort.h"
#include "NodeColorVariable.h"

VariableOutputNode::VariableOutputNode(const ImVec2& CenterPositionIn, const ImVec2& RectangleSizeIn)
	: VariableNode(
		CenterPositionIn,
		RectangleSizeIn,
		NodeColor::VariableNodeBaseColor,
		NodeColor::VariableNodeHilightedColor
	)
{
	AddInputPort<VariableInputPort>(ImVec2(NULL, NULL), 10.f);
}

VariableOutputNode::~VariableOutputNode()
{
}
