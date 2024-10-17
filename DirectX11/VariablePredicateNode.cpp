#include "VariablePredicateNode.h"
#include "InputPort.h"
#include "VariableOutputPort.h"
#include "GlobalVariable.h"

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
		AddInputPort<InputPort>(ImVec2(NULL, NULL),
			10.f,
			NodeColor::VariablePortBaseColor,
			NodeColor::VariablePortHilightedColor
		);
	}

	AddOutputPort<VariableOutputPort>(ImVec2(NULL, NULL), 10.f);
}

VariablePredicateNode::~VariablePredicateNode()
{
}
