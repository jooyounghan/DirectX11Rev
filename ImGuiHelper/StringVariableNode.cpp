#include "StringVariableNode.h"
#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;


StringVariableNode::StringVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<string, StringVariableOutputPort>(
		"String", leftTop, radius, referencedOrigin
	)
{
}

void StringVariableNode::DrawImpl(ImDrawList* drawListIn)
{
	VariableNode<string, StringVariableOutputPort>::DrawImpl(drawListIn);

	const ImVec2& drawNodeFieldPos = GetDrawNodeFieldPos();
	const ImVec2& drawNodeFieldSize = GetDrawNodeFieldSize();
	const ImVec2 drawMarginedFieldPos = ImVec2(drawNodeFieldPos.x + nodeMargin, drawNodeFieldPos.y + nodeMargin);
	SetCursorScreenPos(drawMarginedFieldPos);

	ImGuiIO& io = ImGui::GetIO();

	static char buffer[1024];
	SetNextItemAllowOverlap();
	PushItemWidth(drawNodeFieldSize.x - 2.f * nodeMargin);
	PushID(format("{}", (uint64_t)this).c_str());
	InputText("", buffer, 1024);
	if (IsItemActive()) io.WantCaptureMouse = false;
	string str(buffer);
	m_variableOutputPort.SetString(str);
	m_textInputSize = CalcTextSize(str.c_str());
	PopID();
	PopItemWidth();
}

ImVec2 StringVariableNode::GetInternalNodeSize()
{
	const ImVec2 internalNodeSize = VariableNode<string, StringVariableOutputPort>::GetInternalNodeSize();

	return ImVec2(
		max(m_textInputSize.x + nodeMargin * 2.f, internalNodeSize.x),
		max(m_textInputSize.y + nodeMargin * 2.f, internalNodeSize.y)
	);
}
