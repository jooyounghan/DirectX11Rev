#include "Float3VariableNode.h"
#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace DirectX;
using namespace ImGui;

Float3VariableNode::Float3VariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<XMFLOAT3>("Float3", leftTop, radius, referencedOrigin, {})
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			SetItemCursorWithInternalMargin(drawLeftTop);
			DrawDragFloat3();
			return GetItemRectWithInternalMargin();
		}
	);
}

void Float3VariableNode::DrawDragFloat3()
{
	SetNextItemAllowOverlap();

	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	constexpr float defaultFloat3Widht = defaultDragWidth * 3.f;

	PushItemWidth(dynamicWidth > defaultFloat3Widht ? dynamicWidth : defaultFloat3Widht);
	PushID(format("{}", (uint64_t)this).c_str());
	DragFloat3("", &m_float3.x);
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}
