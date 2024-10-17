#include "VariablePredicateNode.h"
#include "VariableInputPort.h"
#include "VariableOutputPort.h"
#include "NodeColorVariable.h"

VariablePredicateNode::VariablePredicateNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	const size_t InputVariableCountIn
)
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

	AddOutputPort<VariableOutputPort>(ImVec2(NULL, NULL), 10.f);
}

VariablePredicateNode::~VariablePredicateNode()
{
}
