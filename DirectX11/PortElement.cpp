#include "PortElement.h"

constexpr float HilightPadding = 5.f;

using namespace ImGui;

PortElement::PortElement(
	const ImVec2& CenterPositionIn,
	const float& RadiusSizeIn,
	const ImU32& BasePortColorIn, 
	const ImU32& HilightedPortColorIn
)
	: ADrawElement(BasePortColorIn, HilightedPortColorIn)
{
	CenterPosition = CenterPositionIn;
	RadiusSize = RadiusSizeIn;
}

PortElement::~PortElement()
{
}

void PortElement::SetPosition(const ImVec2& CenterPositionIn)
{
	CenterPosition = CenterPositionIn;
}

void PortElement::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	const ImVec2 RecalcedCenter = ImVec2(CenterPosition.x + OriginPosition.x, CenterPosition.y + OriginPosition.y);

	if (bIsConnecting)
	{
		DrawListIn->AddCircleFilled(RecalcedCenter, RadiusSize + HilightPadding, HilightedColor, 20.f);
	}

	DrawListIn->AddCircleFilled(RecalcedCenter, RadiusSize, BaseColor, 20.f);

	if (IsHoveringOnPort(OriginPosition))
	{
		if (!bIsMouseEntered)
		{
			bIsMouseEntered = true;
			MouseEnterEvent.Invoke(this);
		}
		
		if (IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			ClickedEvent.Invoke(this);
		}

		if (IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			MouseDownEvent.Invoke(this);
		}

		if (IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			MouseUpEvent.Invoke(this);
		}
	}
	else
	{
		if (bIsMouseEntered)
		{
			MouseLeaveEvent.Invoke(this);
		}
	}

}

bool PortElement::IsHoveringOnPort(const ImVec2& OriginPosition)
{
	const ImVec2& MousePos = GetMousePos();
	const ImVec2 CanvasMousePos(MousePos.x - OriginPosition.x, MousePos.y - OriginPosition.y);

	float Distance = sqrtf(powf(CanvasMousePos.x - CenterPosition.x, 2) + powf(CanvasMousePos.y - CenterPosition.y, 2));

	if (Distance < RadiusSize)
	{
		ImGui::SetTooltip("Mouse is hovering over the circle!");
		return true;
	}
	return false;
}
