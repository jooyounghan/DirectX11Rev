#include "InputPort.h"
#include "AOutputPort.h"

InputPort::InputPort(
	const ImVec2& CenterPositionIn,
	const float& RadiusSizeIn,
	const ImU32& BasePortColorIn,
	const ImU32& HilightedPortColorIn
)
	: PortElement(CenterPositionIn, RadiusSizeIn, BasePortColorIn, HilightedPortColorIn)
{
}

InputPort::~InputPort()
{
}

void InputPort::Connect(AOutputPort* OutputPortIn)
{
	AOutputPort* LastConnectedOutputPort = ConnectedOutputPort;
	if (LastConnectedOutputPort != nullptr)
	{
		ConnectedOutputPort = nullptr;
		LastConnectedOutputPort->Connect(nullptr);
	}
	ConnectedOutputPort = OutputPortIn;
	ConnectEvent.Invoke(OutputPortIn);
}

void InputPort::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	PortElement::AddToDrawList(OriginPosition, DrawListIn);
}
