#include "VariableOutputPort.h"
#include "VariableNode.h"
#include "VariableInputPort.h"
#include "NodeColorVariable.h"

VariableOutputPort::VariableOutputPort(
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

VariableOutputPort::~VariableOutputPort()
{
}

bool VariableOutputPort::IsConnectable(InputPort* PortIn)
{
	VariableInputPort* VariableInputNodeCached = dynamic_cast<VariableInputPort*>(PortIn);
	if (VariableInputNodeCached != nullptr)
	{
		return true;
	}
	return false;
}
