#include "Node.h"
#include "DrawElementColor.h"
#include "NodeConstant.h"

using namespace std;
using namespace ImGui;

Node::Node(
	const string& nodeName, const ImVec2& leftTop,
	const ImVec2& referencedOrigin, const ImU32& baseColor
)
	: ADrawElement(referencedOrigin, baseColor),
	m_nodeName(nodeName), m_leftTop(leftTop)
{
	m_headerTextSize = CalcTextSize(m_nodeName.c_str());

	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			m_headerSize = ImVec2(m_totalSize.x > m_headerTextSize.x ? m_totalSize.x : m_headerTextSize.x, m_headerTextSize.y);
			ImVec2 headerRightBottom = ImVec2(drawLeftTop.x + m_headerSize.x, drawLeftTop.y + m_headerSize.y);
			drawListIn->AddRectFilled(drawLeftTop, headerRightBottom, m_baseColor, defaultRounding);

			const ImVec2 headerPos = ImVec2(drawLeftTop.x + (m_totalSize.x - m_headerTextSize.x) / 2.f, drawLeftTop.y);
			drawListIn->AddText(headerPos, textColor, m_nodeName.c_str());

			return m_headerSize;
		});
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

void Node::Draw(ImDrawList* drawListIn)
{
	m_drawLeftTop = ImVec2(m_leftTop.x + m_referencedOrigin.x, m_leftTop.y + m_referencedOrigin.y);

	const ImVec2 drawRightBottom = ImVec2(m_drawLeftTop.x + m_totalSize.x, m_drawLeftTop.y + m_totalSize.y);
	drawListIn->AddRectFilled(m_drawLeftTop, drawRightBottom, m_baseColor, defaultRounding);

	ImVec2 nodeSize = ImVec2(0.f, 0.f);
	ImVec2 drawLeftTop = m_drawLeftTop;
	for (auto& drawCommands : m_drawsCommands)
	{
		float widthSize = 0.f;
		float heightSize = 0.f;
		ImVec2 drawLeftTopPerItem = ImVec2(drawLeftTop.x, drawLeftTop.y + nodeSize.y);
		for (auto& drawCommand : drawCommands)
		{
			const ImVec2 itemSize = drawCommand(drawLeftTopPerItem, drawListIn);
			widthSize += itemSize.x;
			drawLeftTopPerItem.x += itemSize.x;
			heightSize = max(heightSize, itemSize.y);
		}

		nodeSize.x = max(nodeSize.x, widthSize);
		nodeSize.y += heightSize;
	}

	m_totalSize = nodeSize;
}

void Node::AddDrawCommand(const ImGuiDrawFunction& drawCommand)
{
	m_drawsCommands.push_back(std::vector<ImGuiDrawFunction>{drawCommand});
}

void Node::AddDrawCommandSameLine(const ImGuiDrawFunction& drawCommand)
{
	m_drawsCommands.at(m_drawsCommands.size() - 1).push_back(drawCommand);
}

void Node::OnMouseClicked(MouseClickEventArgs& args)
{
	ADrawElement::OnMouseClicked(args);
	m_offsetFromLeftTop = ImVec2(
		args.m_mousePosX - m_drawLeftTop.x,
		args.m_mousePosY - m_drawLeftTop.y
	);
}

void Node::OnDragging(MouseEventArgs& args)
{
	ADrawElement::OnDragging(args);
	m_leftTop = ImVec2(
		args.m_mousePosX - m_referencedOrigin.x - m_offsetFromLeftTop.x,
		args.m_mousePosY - m_referencedOrigin.y - m_offsetFromLeftTop.y
	);
}

