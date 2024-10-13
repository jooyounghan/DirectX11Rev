#include "ADrawElement.h"

ADrawElement::ADrawElement(const ImU32& BasePortColorIn, const ImU32& HilightedPortColorIn)
	: BaseColor(BasePortColorIn), HilightedColor(HilightedPortColorIn)
{
}

ADrawElement::~ADrawElement()
{
}

void ADrawElement::SetFocus(const bool& IsFocused)
{
	bIsElementFocused = IsFocused;
}
