#include "VariableInputNode.h"
#include "VariableOutputPort.h"
#include "NodeColorVariable.h"

VariableInputNode::VariableInputNode(const ImVec2& CenterPositionIn, const ImVec2& RectangleSizeIn)
	: VariableNode(
		CenterPositionIn, 
		RectangleSizeIn,
		NodeColor::VariableNodeBaseColor,
		NodeColor::VariableNodeHilightedColor
	)
{
}

VariableInputNode::~VariableInputNode()
{
}
