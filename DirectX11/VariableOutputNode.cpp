#include "VariableOutputNode.h"
#include "InputPort.h"
#include "GlobalVariable.h"

VariableOutputNode::VariableOutputNode(const ImVec2& CenterPositionIn, const ImVec2& RectangleSizeIn)
	: NodeElement(
		CenterPositionIn,
		RectangleSizeIn,
		NodeColor::VariableNodeBaseColor,
		NodeColor::VariableNodeHilightedColor
	)
{
	AddInputPort<InputPort>(ImVec2(NULL, NULL),
		10.f,
		NodeColor::VariablePortBaseColor,
		NodeColor::VariablePortHilightedColor
	);
}

VariableOutputNode::~VariableOutputNode()
{
}
