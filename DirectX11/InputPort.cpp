#include "InputPort.h"

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

void InputPort::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	PortElement::AddToDrawList(OriginPosition, DrawListIn);
}
