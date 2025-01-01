#include "ADrawElement.h"
#include "DrawElementManager.h"

ADrawElement::ADrawElement(const ImVec2& referencedOrigin,
	const ImU32& baseColor, const ImU32& hoveringColor,
	const ImU32& borderColor, const ImU32& hilightedBoderColor
)
	: m_referencedOrigin(referencedOrigin),
	m_baseColor(baseColor), m_hoveringColor(hoveringColor), 
	m_borderColor(borderColor), m_hilightedBorderColor(hilightedBoderColor),
	m_selectedColor(m_baseColor), m_selectedBorderColor(m_borderColor)
{

}

void ADrawElement::AddToDrawElementManager(DrawElementManager* drawElementManager)
{
	drawElementManager->AddDrawElement(this);
}

void ADrawElement::RemoveFromDrawElementManager(DrawElementManager* drawElementManager)
{
	drawElementManager->RemoveDrawElement(this);
}

void ADrawElement::SetHilighted(const bool& isHilighted)
{
	m_isHilghted = isHilighted;
	m_selectedBorderColor = m_isHilghted ? m_hilightedBorderColor : m_borderColor;
}

void ADrawElement::Draw(ImDrawList* drawListIn)
{
	AdjustPosition();
	DrawImpl(drawListIn);
}

void ADrawElement::OnMouseClicked(MouseClickEventArgs& args)
{
	SetHilighted(true);
	args.m_isHandled = true;
}

void ADrawElement::OnMouseDown(MouseClickEventArgs& args)
{
	args.m_isHandled = true;
}

void ADrawElement::OnMouseEnter(MouseEventArgs& args)
{
	m_selectedColor = m_hoveringColor;
}

void ADrawElement::OnMouseLeave(MouseEventArgs& args)
{
	m_selectedColor = m_baseColor;
}