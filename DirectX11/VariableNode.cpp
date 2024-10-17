#include "VariableNode.h"

VariableNode::VariableNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	const ImU32& RectangleColorIn, 
	const ImU32& HilightedColorIn
)
	: NodeElement(CenterPositionIn, RectangleSizeIn, RectangleColorIn, HilightedColorIn)
{
}

VariableNode::~VariableNode()
{
}
