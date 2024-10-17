#include "FlowOutputPort.h"
#include "FlowNode.h"
#include "FlowInputPort.h"
#include "NodeColorVariable.h"

FlowOutputPort::FlowOutputPort(
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

FlowOutputPort::~FlowOutputPort()
{
}

bool FlowOutputPort::IsConnectable(InputPort* PortIn)
{
	FlowInputPort* FlowInputNodeCached = dynamic_cast<FlowInputPort*>(PortIn);
	if (FlowInputNodeCached != nullptr)
	{
		return true;
	}
	return false;
}
