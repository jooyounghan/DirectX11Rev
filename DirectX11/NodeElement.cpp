#include "NodeElement.h"
#include <format>

using namespace std;

NodeElement::NodeElement(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	const ImU32& RectangleColorIn, 
	const ImU32& HilightedColorIn
)
	: RectangleDrawElement(CenterPositionIn, RectangleSizeIn, RectangleColorIn, HilightedColorIn),
	NodeID(string{ format("{}", (uint64_t)this) })
{
}

NodeElement::~NodeElement()
{
}

void NodeElement::SetPosition(const ImVec2& CenterPosition)
{
	RectangleDrawElement::SetPosition(CenterPosition);

	float PortCount = 0.f;
	const float InputPortTotalCount = static_cast<float>(InputPorts.size());
	for (auto& InputPortInstance : InputPorts)
	{
		PortCount += 1.f;
		const ImVec2 PortPosition = ImVec2(
			CenterPosition.x - (ElementSize.x / 2.f),
			CenterPosition.y - (ElementSize.y / 2.f) + (ElementSize.y * (PortCount) / (InputPortTotalCount + 1.f))
		);
		InputPortInstance->SetPosition(PortPosition);
	}
	PortCount = 0.f;
	const float OutputPortTotalCount = static_cast<float>(OutputPorts.size());
	for (auto& OutputPortInstance : OutputPorts)
	{
		PortCount += 1.f;
		const ImVec2 PortPosition = ImVec2(
			CenterPosition.x + (ElementSize.x / 2.f),
			CenterPosition.y - (ElementSize.y / 2.f) + (ElementSize.y * (PortCount) / (OutputPortTotalCount + 1.f))
		);
		OutputPortInstance->SetPosition(PortPosition);
	}
}

void NodeElement::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	RectangleDrawElement::AddToDrawList(OriginPosition, DrawListIn);
	for (auto& InputPortInstance : InputPorts)
	{
		InputPortInstance->AddToDrawList(OriginPosition, DrawListIn);
	}
	for (auto& OutputPortInstance : OutputPorts)
	{
		OutputPortInstance->AddToDrawList(OriginPosition, DrawListIn);
	}
}
