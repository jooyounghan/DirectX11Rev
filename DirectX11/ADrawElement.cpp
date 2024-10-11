#include "ADrawElement.h"

void ADrawElement::SetPosition(const ImVec2& CenterPositionIn)
{
	LeftTopPosition = ImVec2(CenterPositionIn.x - ElementSize.x / 2.f, CenterPositionIn.y - ElementSize.y / 2.f);
	RightBottomPosition = ImVec2(LeftTopPosition.x + ElementSize.x, LeftTopPosition.y + ElementSize.y);
}

void ADrawElement::SetPosition(const ImVec2& CenterPositionIn, const ImVec2& ElementSizeIn)
{
	LeftTopPosition = ImVec2(CenterPositionIn.x - ElementSizeIn.x / 2.f, CenterPositionIn.y - ElementSizeIn.y / 2.f);
	ElementSize = ElementSizeIn;
	RightBottomPosition = ImVec2(LeftTopPosition.x + ElementSize.x, LeftTopPosition.y + ElementSize.y);
}

void ADrawElement::SetFocus(const bool& IsFocused)
{
	bIsElementFocused = IsFocused;
}
