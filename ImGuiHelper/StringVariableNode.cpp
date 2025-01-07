#include "StringVariableNode.h"
#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;


StringVariableNode::StringVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<string>("String", leftTop, radius, referencedOrigin, {})
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			SetItemCursorWithInternalMargin(drawLeftTop);
			DrawStringEdit();
			return GetItemRectWithInternalMargin();
		}
	);
}
void StringVariableNode::DrawStringEdit()
{
	static char buffer[1024];

	SetNextItemAllowOverlap();
	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultTextSize ? dynamicWidth : defaultTextSize);
	PushID(format("{}", (uint64_t)this).c_str());
	InputText("", buffer, 1024);
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();

	m_string = string(buffer);
}