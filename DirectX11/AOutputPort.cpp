#include "AOutputPort.h"
#include "InputPort.h"
#include "NodeElement.h"

using namespace ImGui;

AOutputPort::AOutputPort(
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

AOutputPort::~AOutputPort()
{
}

void AOutputPort::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
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

void AOutputPort::AddConnectionLine(const ImVec2& OriginPosition, const ImVec2& ToPosition, ImDrawList* DrawListIn)
{
	const ImVec2 Point1 = ImVec2(CenterPosition.x + OriginPosition.x, CenterPosition.y + OriginPosition.y);
	const ImVec2 Point4 = ToPosition;

	const ImVec2 Point2 = ImVec2(Point1.x + 100.f, Point1.y);
	const ImVec2 Point3 = ImVec2(Point4.x - 100.f, Point4.y);

	DrawListIn->AddBezierCubic(Point1, Point2, Point3, Point4, ConnectionLineColor, 5.f);
}

void AOutputPort::Connect(InputPort* PortIn)
{
	if (IsConnectable(PortIn) && !IsCyclic(PortIn))
	{
		ConnectedPort = PortIn;
		ConnectedPort->Connect(this);
	}
	else
	{
		if (ConnectedPort != nullptr)
		{
			ConnectedPort->Connect(nullptr);
		}
		ConnectedPort = nullptr;
	}
}

bool AOutputPort::IsCyclic(InputPort* TargetPort)
{
	if (TargetPort == nullptr)
	{
		return false;
	}

	NodeElement* ParentNode = GetParentNodeElement();
	NodeElement* CurrentParentNode = TargetPort->GetParentNodeElement();

	if (ParentNode == CurrentParentNode)
	{
		return true;
	}

	for (auto& OutputPort : CurrentParentNode->GetOutputPorts())
	{
		if (OutputPort->IsCyclic(ParentNode))
		{
			return true;
		}
	}
	return false;
}

bool AOutputPort::IsCyclic(NodeElement* InitialCheckNode)
{
	if (ConnectedPort != nullptr)
	{
		NodeElement* CurrentParentNode = ConnectedPort->GetParentNodeElement();
		if (CurrentParentNode == InitialCheckNode)
		{
			return true;
		}

		for (auto& OutputPort : CurrentParentNode->GetOutputPorts())
		{
			if (OutputPort->IsCyclic(InitialCheckNode))
			{
				return true;
			}
		}
	}

	return false;
}
