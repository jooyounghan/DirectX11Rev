#include "VariableInputNode.h"
#include "OutputPort.h"
#include "GlobalVariable.h"

VariableInputNode::VariableInputNode(const ImVec2& CenterPositionIn, const ImVec2& RectangleSizeIn)
	: NodeElement(
		CenterPositionIn, 
		RectangleSizeIn,
		NodeColor::VariableNodeBaseColor,
		NodeColor::VariableNodeHilightedColor
	)
{
	AddOutputPort<OutputPort>(ImVec2(NULL, NULL),
		10.f,
		NodeColor::VariablePortBaseColor,
		NodeColor::VariablePortHilightedColor,
		NodeColor::VariablePortHilightedColor
	);
}

VariableInputNode::~VariableInputNode()
{
}
