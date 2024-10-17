#include "RectangleDrawElement.h"

using namespace ImGui;

constexpr float HilightPadding = 5.f;

RectangleDrawElement::RectangleDrawElement(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn,
	const ImU32& BaseColorIn,
	const ImU32& HilightedColorIn
)
	: ADrawElement(BaseColorIn, HilightedColorIn)
{
	ElementSize = RectangleSizeIn;
	LeftTopPosition = ImVec2(CenterPositionIn.x - ElementSize.x / 2.f, CenterPositionIn.y - ElementSize.y / 2.f);
	RightBottomPosition = ImVec2(LeftTopPosition.x + ElementSize.x, LeftTopPosition.y + ElementSize.y);
}

RectangleDrawElement::~RectangleDrawElement()
{
}

void RectangleDrawElement::SetPosition(const ImVec2& CenterPositionIn)
{
	LeftTopPosition = ImVec2(CenterPositionIn.x - ElementSize.x / 2.f, CenterPositionIn.y - ElementSize.y / 2.f);
	RightBottomPosition = ImVec2(LeftTopPosition.x + ElementSize.x, LeftTopPosition.y + ElementSize.y);
}

const ImVec2& RectangleDrawElement::GetPosition()
{
	return ImVec2(LeftTopPosition.x + ElementSize.x / 2.f, LeftTopPosition.y + ElementSize.y / 2.f);
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

	DrawListIn->AddRectFilled(RecalcedLT, RecalcedRB, BaseColor, ElementSize.x / 20.f);

	if (IsMouseHoveringRect(RecalcedLT, RecalcedRB))
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

		if (IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
		{
			MouseUpEvent.Invoke(this);
		}
	}
	else
	{
		if (bIsMouseEntered)
		{
			bIsMouseEntered = false;
			MouseLeaveEvent.Invoke(this);
		}
	}
}
