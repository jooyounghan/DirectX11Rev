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
	const ImVec2 drawMarginedFieldPos = ImVec2(drawNodeFieldPos.x + nodeMargin, drawNodeFieldPos.y + nodeMargin);
	SetCursorScreenPos(drawMarginedFieldPos);

	ImGuiIO& io = ImGui::GetIO();

	SetNextItemAllowOverlap();
	PushItemWidth(fundamentalNodeItemWidth);
	PushID(format("{}", (uint64_t)this).c_str());
	DragFloat("", &m_float);
	if (IsItemActive()) io.WantCaptureMouse = false;
	PopID();
	PopItemWidth();

	m_dragFloatItemSize = GetItemRectSize();
}

ImVec2 FloatVariableNode::GetInternalNodeSize()
{
	const ImVec2 internalNodeSize = VariableNode<float>::GetInternalNodeSize();

	return ImVec2(
		max(m_dragFloatItemSize.x + nodeMargin * 2.f, internalNodeSize.x),
		max(m_dragFloatItemSize.y + nodeMargin * 2.f, internalNodeSize.y)
	);
}
