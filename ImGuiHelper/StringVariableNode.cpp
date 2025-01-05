#include "StringVariableNode.h"
#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;


StringVariableNode::StringVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<string>(
		"String", leftTop, radius, referencedOrigin
	)
{
}

void StringVariableNode::DrawImpl(ImDrawList* drawListIn)
{
	VariableNode<string>::DrawImpl(drawListIn);

	const ImVec2& drawNodeFieldPos = GetDrawNodeFieldPos();
	const ImVec2& drawNodeFieldSize = GetDrawNodeFieldSize();
	const ImVec2 drawMarginedFieldPos = ImVec2(drawNodeFieldPos.x + nodeInternalMargin, drawNodeFieldPos.y + nodeInternalMargin);
	SetCursorScreenPos(drawMarginedFieldPos);

	ImGuiIO& io = ImGui::GetIO();

	static char buffer[1024];
	SetNextItemAllowOverlap();
	PushItemWidth(drawNodeFieldSize.x - 2.f * nodeInternalMargin);
	PushID(format("{}", (uint64_t)this).c_str());
	InputText("", buffer, 1024);
	if (IsItemActive()) io.WantCaptureMouse = false;
	string str(buffer);
	PopID();
	PopItemWidth();

	m_textInputSize = CalcTextSize(str.c_str());
	m_string = str;
}

void StringVariableNode::UpdateFieldSize()
{
	VariableNode<string>::UpdateFieldSize();
	m_nodeFieldSize = ImVec2(
		max(m_textInputSize.x + nodeInternalMargin * 2.f, m_nodeFieldSize.x),
		max(m_textInputSize.y + nodeInternalMargin * 2.f, m_nodeFieldSize.y)
	);
}