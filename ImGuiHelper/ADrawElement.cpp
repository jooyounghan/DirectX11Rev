#include "ADrawElement.h"
#include "DrawElementManager.h"
#include "DrawElementColor.h"

using namespace std;
using namespace ImGui;

ADrawElement::ADrawElement(const ImVec2& referencedOrigin, const ImU32& baseColor)
	: m_referencedOrigin(referencedOrigin),
	m_baseColor(baseColor), m_selectedBorderFill(borderFill)
{

}

void ADrawElement::ConsumeMouseCaptureForItem() 
{
	ImGuiIO& io = ImGui::GetIO();
	if (IsItemActive()) io.WantCaptureMouse = false;
}

void ADrawElement::SetFocused(const bool& isFocused)
{
	m_isFocused = isFocused;
	if (m_isFocused) OnFocused(this);
}

void ADrawElement::OnMouseClicked(MouseClickEventArgs& args)
{
	SetFocused(true);
	args.m_isHandled = true;
}

void ADrawElement::OnMouseDown(MouseClickEventArgs& args)
{
	args.m_isHandled = true;
}

void ADrawElement::OnMouseEnter(MouseEventArgs& args)
{
	m_selectedBorderFill = borderFillHilighted;
}

void ADrawElement::OnMouseLeave(MouseEventArgs& args)
{
	m_selectedBorderFill = borderFill;
}