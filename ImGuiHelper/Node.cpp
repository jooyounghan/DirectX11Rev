#include "Node.h"
#include "DrawElementColor.h"

using namespace std;
using namespace ImGui;

float Node::nodeMinWidth = 100.f;
float Node::nodeMinHeight = 50.f;

Node::Node(
	const string& nodeName, const ImVec2& leftTop,
	const ImVec2& referencedOrigin, const ImU32& baseColor
)
	: ADrawElement(referencedOrigin, baseColor),
	m_nodeName(nodeName), m_leftTop(leftTop)
{
}

bool Node::IsPointIn(const float& pointX, const float& pointY) const
{
	static auto inRange = [](const float& value, const float& min, const float& max) 
	{
		return min <= value && value <= max;
	};

	return inRange(pointX, m_drawLeftTop.x, m_drawLeftTop.x + m_totalSize.x) &&
		inRange(pointY, m_drawLeftTop.y, m_drawLeftTop.y + m_totalSize.y);
}

void Node::DrawImpl(ImDrawList* drawListIn)
{
	const float radius = m_totalSize.x / 20.f;

	const ImVec2 nodeRightBottom = ImVec2(m_drawLeftTop.x + m_totalSize.x, m_drawLeftTop.y + m_totalSize.y);
	drawListIn->AddRectFilled(m_drawLeftTop, nodeRightBottom, m_isHilghted ? borderFillHilighted : borderFill, radius, NULL);
	drawListIn->AddRect(m_drawLeftTop, nodeRightBottom, borderFillHilighted, radius, NULL, 2.f);

	// Draw Node Name
	const ImVec2 nodeNameRightBottom = ImVec2(m_drawLeftTop.x + m_nodeHeaderSize.x, m_drawLeftTop.y + m_nodeHeaderSize.y);
	drawListIn->AddRectFilled(m_drawLeftTop, nodeNameRightBottom, m_baseColor, radius);
	drawListIn->AddText(m_drawNodeHeaderPos, textColor, m_nodeName.c_str());
}

void Node::AdjustPosition()
{
	m_drawLeftTop = ImVec2(m_leftTop.x + m_referencedOrigin.x, m_leftTop.y + m_referencedOrigin.y);

	m_nodeHeaderSize = CalcTextSize(m_nodeName.c_str());
	m_nodeFieldSize = GetInternalNodeSize();

	m_totalSize = ImVec2(
		std::max(m_nodeHeaderSize.x, m_nodeFieldSize.x),
		m_nodeHeaderSize.y + m_nodeFieldSize.y
	);

	m_drawNodeHeaderPos = ImVec2(m_drawLeftTop.x + (m_totalSize.x - m_nodeHeaderSize.x) / 2.f, m_drawLeftTop.y);
	m_drawNodeFieldPos = ImVec2(m_drawLeftTop.x, m_drawLeftTop.y + m_nodeHeaderSize.y);

	m_nodeHeaderSize.x = m_totalSize.x;
	m_nodeFieldSize.x = m_totalSize.x;
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
		args.m_mousePosX - m_totalSize.x / 2.f - m_referencedOrigin.x,
		args.m_mousePosY - m_totalSize.y / 2.f - m_referencedOrigin.y
	);
}

