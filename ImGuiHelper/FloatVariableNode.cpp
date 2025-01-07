#include "FloatVariableNode.h"
#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

FloatVariableNode::FloatVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<float>("Float", leftTop, radius, referencedOrigin, {})
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			SetItemCursorWithInternalMargin(drawLeftTop);
			DrawDragFloat();
			return GetItemRectWithInternalMargin();
		}
	);
}

void FloatVariableNode::DrawDragFloat()
{
	SetNextItemAllowOverlap();

	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultDragWidth ? dynamicWidth : defaultDragWidth);
	PushID(format("{}", (uint64_t)this).c_str());
	DragFloat("", &m_float);
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}