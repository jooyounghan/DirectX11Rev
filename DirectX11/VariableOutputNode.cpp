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
}

VariableOutputNode::~VariableOutputNode()
{
}
