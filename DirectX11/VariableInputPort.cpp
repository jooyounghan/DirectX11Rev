#include "VariableInputPort.h"
#include "NodeColorVariable.h"

VariableInputPort::VariableInputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn
)
	: InputPort(CenterPositionIn, RadiusSizeIn, 
		NodeColor::VariablePortBaseColor,
		NodeColor::VariablePortHilightedColor
	)
{
}

VariableInputPort::~VariableInputPort()
{
}
