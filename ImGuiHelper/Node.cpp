#include "Node.h"

Node::Node(
	const ImVec2& leftTop, const ImVec2& size,
	const ImVec2& referencedOrigin,
	const ImU32& baseColor, const ImU32& hoveringColor,
	const ImU32& borderColor, const ImU32& hilightedBoderColor
)
	: ADrawElement(
		referencedOrigin, 
		baseColor, hoveringColor, 
		borderColor, hilightedBoderColor
	), m_leftTop(leftTop), m_size(size)
{
}

bool Node::IsPointIn(const float& pointX, const float& pointY) const
{
	return m_drawLeftTop.x <= pointX && pointX <= m_drawRightBottom.x && m_drawLeftTop.y <= pointY && pointY <= m_drawRightBottom.y;
}

void Node::DrawImpl(ImDrawList* drawListIn)
{
	const float radius = m_size.x / 20.f;
	drawListIn->AddRectFilled(m_drawLeftTop, m_drawRightBottom, *m_selectedColor, radius);
	drawListIn->AddRect(m_drawLeftTop, m_drawRightBottom, *m_selectedBorderColor, radius, NULL, 2.f);
}

void Node::AdjustPosition()
{
	m_rightBottom.x = m_leftTop.x + m_size.x;
	m_rightBottom.y = m_leftTop.y + m_size.y;

	m_drawLeftTop = ImVec2(m_leftTop.x + m_referencedOrigin.x, m_leftTop.y + m_referencedOrigin.y);
	m_drawRightBottom = ImVec2(m_rightBottom.x + m_referencedOrigin.x, m_rightBottom.y + m_referencedOrigin.y);

}

void Node::OnMouseClicked(MouseClickEventArgs& args)
{
	ADrawElement::OnMouseClicked(args);
	OnMouseClickedHandler(this);
	args.m_isHandled = true;
}

void Node::OnDragging(MouseEventArgs& args)
{
	ADrawElement::OnDragging(args);
	m_leftTop = ImVec2(
		args.m_mousePosX - m_size.x / 2.f - m_referencedOrigin.x, 
		args.m_mousePosY - m_size.y / 2.f - m_referencedOrigin.y
	);
}

