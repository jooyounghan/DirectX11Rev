#include "VariableOutputPort.h"
#include "VariableNode.h"
#include "InputPort.h"
#include "GlobalVariable.h"

VariableOutputPort::VariableOutputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn
)
	: AOutputPort(
		CenterPositionIn, RadiusSizeIn,
		NodeColor::FlowPortBaseColor,
		NodeColor::FlowPortHilightedColor,
		NodeColor::FlowPortHilightedColor
	)
{
}

VariableOutputPort::~VariableOutputPort()
{
}

bool VariableOutputPort::IsConnectable(InputPort* PortIn)
{
	if (PortIn != nullptr)
	{
		VariableNode* VariableNodeCached = dynamic_cast<VariableNode*>(PortIn->GetParentNodeElement());
		if (VariableNodeCached != nullptr)
		{
			return true;
		}
	}
	return false;
}
