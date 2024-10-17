#include "OutputPort.h"
#include "InputPort.h"

using namespace ImGui;

OutputPort::OutputPort(
	const ImVec2& CenterPositionIn,
	const float& RadiusSizeIn,
	const ImU32& BasePortColorIn,
	const ImU32& HilightedPortColorIn,
	const ImU32& ConnectionLineColorIn
)
	: PortElement(CenterPositionIn, RadiusSizeIn, BasePortColorIn, HilightedPortColorIn),
	ConnectionLineColor(ConnectionLineColorIn)
{
}

OutputPort::~OutputPort()
{
}

void OutputPort::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	// DrawConnectionLine
	if (bIsConnecting)
	{
		AddConnectionLine(OriginPosition, GetMousePos(), DrawListIn);
	}

	if (ConnectedPort != nullptr)
	{
		const ImVec2 ConnectedPosition = ConnectedPort->GetPosition();

		AddConnectionLine(OriginPosition, ImVec2(OriginPosition.x + ConnectedPosition.x, OriginPosition.y + ConnectedPosition.y), DrawListIn);
	}
	PortElement::AddToDrawList(OriginPosition, DrawListIn);
}

void OutputPort::AddConnectionLine(const ImVec2& OriginPosition, const ImVec2& ToPosition, ImDrawList* DrawListIn)
{
	const ImVec2 Point1 = ImVec2(CenterPosition.x + OriginPosition.x, CenterPosition.y + OriginPosition.y);
	const ImVec2 Point4 = ToPosition;

	const ImVec2 Point2 = ImVec2(Point1.x + 100.f, Point1.y);
	const ImVec2 Point3 = ImVec2(Point4.x - 100.f, Point4.y);

	DrawListIn->AddBezierCubic(Point1, Point2, Point3, Point4, ConnectionLineColor, 5.f);
}

void OutputPort::Connect(InputPort* PortIn)
{
	ConnectedPort = PortIn;
}
