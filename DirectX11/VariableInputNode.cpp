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
	AddOutputPort<VariableOutputPort>(ImVec2(NULL, NULL), 10.f);
}

VariableInputNode::~VariableInputNode()
{
}
