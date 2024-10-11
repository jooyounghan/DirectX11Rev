#include "RectangleDrawElement.h"

using namespace ImGui;

constexpr float HilightPadding = 5.f;

RectangleDrawElement::RectangleDrawElement(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn,
	const ImU32& RectangleColorIn,
	const ImU32& HilightedColorIn
)
{
	RectangleColor = RectangleColorIn;
	HilightedColor = HilightedColorIn;
	SetPosition(CenterPositionIn, RectangleSizeIn);
}

RectangleDrawElement::~RectangleDrawElement()
{
}

void RectangleDrawElement::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	const ImVec2 RecalcedLT = ImVec2(LeftTopPosition.x + OriginPosition.x, LeftTopPosition.y + OriginPosition.y);
	const ImVec2 RecalcedRB = ImVec2(RightBottomPosition.x + OriginPosition.x, RightBottomPosition.y + OriginPosition.y);

	if (bIsElementFocused)
	{
		ImVec2 LeftTopPositionWithPadding = ImVec2(RecalcedLT.x - HilightPadding, RecalcedLT.y - HilightPadding);
		ImVec2 RightBottomPositionWithPadding = ImVec2(RecalcedRB.x + HilightPadding, RecalcedRB.y + HilightPadding);
		DrawListIn->AddRectFilled(LeftTopPositionWithPadding, RightBottomPositionWithPadding, HilightedColor, ElementSize.x / 20.f);
	}

	DrawListIn->AddRectFilled(RecalcedLT, RecalcedRB, RectangleColor, ElementSize.x / 20.f);


	if (IsMouseHoveringRect(RecalcedLT, RecalcedRB))
	{
		if (IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			DrawElementDragEvent.Invoke(this, GetMousePos());
		}
		if (IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			SetFocus(true);
			DrawElementClickedEvent.Invoke(this);
		}
	}
}
