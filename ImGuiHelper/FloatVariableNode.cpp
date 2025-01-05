#include "FloatVariableNode.h"
#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

FloatVariableNode::FloatVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<float>(
		"Float", leftTop, radius, referencedOrigin
	)
{
}

void FloatVariableNode::DrawImpl(ImDrawList* drawListIn)
{
	VariableNode<float>::DrawImpl(drawListIn);

	const ImVec2& drawNodeFieldPos = GetDrawNodeFieldPos();
	const ImVec2& drawNodeFieldSize = GetDrawNodeFieldSize();
	const ImVec2 drawMarginedFieldPos = ImVec2(drawNodeFieldPos.x + nodeInternalMargin, drawNodeFieldPos.y + nodeInternalMargin);
	SetCursorScreenPos(drawMarginedFieldPos);

	ImGuiIO& io = ImGui::GetIO();

	SetNextItemAllowOverlap();
	PushItemWidth(drawNodeFieldSize.x - 2.f * nodeInternalMargin);
	PushID(format("{}", (uint64_t)this).c_str());
	DragFloat("", &m_float);
	if (IsItemActive()) io.WantCaptureMouse = false;
	PopID();
	PopItemWidth();

	m_dragFloatItemSize = GetItemRectSize();
}

void FloatVariableNode::UpdateFieldSize()
{
	VariableNode<float>::UpdateFieldSize();
	m_nodeFieldSize = ImVec2(
		max(m_dragFloatItemSize.x + nodeInternalMargin * 2.f, m_nodeFieldSize.x),
		max(m_dragFloatItemSize.y + nodeInternalMargin * 2.f, m_nodeFieldSize.y)
	);
}