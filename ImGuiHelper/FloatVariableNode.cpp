#include "FloatVariableNode.h"
#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

FloatVariableNode::FloatVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<float>("Float", leftTop, radius, referencedOrigin)
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			const ImVec2 floatDrawPos = ImVec2(drawLeftTop.x + nodeInternalMargin, drawLeftTop.y + nodeInternalMargin);
			SetCursorScreenPos(floatDrawPos);
			DrawDragFloat();
			const ImVec2 itemSizeWithoutMargin = GetItemRectSize();			

			return ImVec2(itemSizeWithoutMargin.x + 2.f * nodeInternalMargin, itemSizeWithoutMargin.y + 2.f * nodeInternalMargin);
		}
	);
}

void FloatVariableNode::DrawDragFloat()
{
	SetNextItemAllowOverlap();

	PushItemWidth(defaultDragWidth);
	PushID(format("{}", (uint64_t)this).c_str());
	DragFloat("", &m_float);
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}