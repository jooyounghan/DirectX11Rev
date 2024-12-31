#include "ANode.h"


ANode::ANode(const ImVec2& leftTop, const ImVec2& size, const ImVec2& referencedOrigin)
	: ADrawElement(leftTop), m_size(size), m_referencedOrigin(referencedOrigin)
{
}

bool ANode::IsPointIn(const float& pointX, const float& pointY) const
{
	ImVec2 adjustedLeftTop = ImVec2(m_leftTop.x + m_referencedOrigin.x, m_leftTop.y + m_referencedOrigin.y);
	ImVec2 adjustedRightBottom = ImVec2(m_rightBottom.x + m_referencedOrigin.x, m_rightBottom.y + m_referencedOrigin.y);
	return adjustedLeftTop.x <= pointX && pointX <= adjustedRightBottom.x && adjustedLeftTop.y <= pointY && pointY <= adjustedRightBottom.y;
}

void ANode::DrawImpl(ImDrawList* drawListIn)
{
	ImVec2 adjustedLeftTop = ImVec2(m_leftTop.x + m_referencedOrigin.x, m_leftTop.y + m_referencedOrigin.y);
	ImVec2 adjustedRightBottom = ImVec2(m_rightBottom.x + m_referencedOrigin.x, m_rightBottom.y + m_referencedOrigin.y);

	drawListIn->AddRectFilled(adjustedLeftTop, adjustedRightBottom, IM_COL32(255, 50, 50, 255), m_size.x / 20.f);

}

void ANode::AdjustPosition()
{
	m_rightBottom.x = m_leftTop.x + m_size.x;
	m_rightBottom.y = m_leftTop.y + m_size.y;
}

void ANode::OnMouseClicked(MouseClickEventArgs& args)
{
	bool test = true;
}
