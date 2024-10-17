#include "VariablePredicateNode.h"
#include "InputPort.h"
#include "OutputPort.h"
#include "GlobalVariable.h"

VariablePredicateNode::VariablePredicateNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	const size_t InputVariableCountIn
)
	: NodeElement(
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

	AddOutputPort<OutputPort>(ImVec2(NULL, NULL),
		10.f,
		NodeColor::VariablePortBaseColor,
		NodeColor::VariablePortHilightedColor,
		NodeColor::VariablePortHilightedColor
	);
}

VariablePredicateNode::~VariablePredicateNode()
{
}
