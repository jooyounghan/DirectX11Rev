#include "ADrawElement.h"

ADrawElement::ADrawElement(const ImVec2& leftTop)
{
	m_leftTop = leftTop;
}

void ADrawElement::Draw(ImDrawList* drawListIn)
{
	AdjustPosition();
	DrawImpl(drawListIn);
}
