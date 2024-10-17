#include "FlowInputPort.h"
#include "NodeColorVariable.h"

FlowInputPort::FlowInputPort(
	const ImVec2& CenterPositionIn,
	const float& RadiusSizeIn
)
	: InputPort(CenterPositionIn, RadiusSizeIn,
		NodeColor::FlowPortBaseColor,
		NodeColor::FlowPortHilightedColor
	)
{
}

FlowInputPort::~FlowInputPort()
{
}
