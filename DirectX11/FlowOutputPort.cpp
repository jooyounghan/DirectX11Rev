#include "FlowOutputPort.h"
#include "FlowNode.h"
#include "InputPort.h"
#include "GlobalVariable.h"

FlowOutputPort::FlowOutputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn
)
	: AOutputPort(
		CenterPositionIn, RadiusSizeIn,
		NodeColor::VariablePortBaseColor,
		NodeColor::VariablePortHilightedColor,
		NodeColor::VariablePortHilightedColor
	)
{
}

FlowOutputPort::~FlowOutputPort()
{
}

bool FlowOutputPort::IsConnectable(InputPort* PortIn)
{
	if (PortIn != nullptr)
	{
		FlowNode* FlowNodeCached = dynamic_cast<FlowNode*>(PortIn->GetParentNodeElement());
		if (FlowNodeCached != nullptr)
		{
			return true;
		}
	}
	return false;
}
