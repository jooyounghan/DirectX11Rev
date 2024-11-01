#include "FloatInputNode.h"
#include "FloatVariableOutputPort.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

FloatInputNode::FloatInputNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	const float& FloatDataIn
)
	: VariableInputNode(CenterPositionIn, RectangleSizeIn),
	FloatSizeX(ElementSize.x - 2.f * NodePaddingSize),
	FloatSizeY((ElementSize.y - 2.f * NodePaddingSize) / 2.f)
{
	FloatVariableOutputPortCached = AddOutputPort<FloatVariableOutputPort>(ImVec2(NULL, NULL), 10.f);
	FloatVariableOutputPortCached->SetFloatData(FloatDataIn);
}

FloatInputNode::~FloatInputNode()
{
}

void FloatInputNode::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	NodeElement::AddToDrawList(OriginPosition, DrawListIn);

	const ImVec2 FloatDescPosition = ImVec2(
		LeftTopPosition.x + OriginPosition.x + NodePaddingSize,
		LeftTopPosition.y + OriginPosition.y + NodePaddingSize
	);
	const ImVec2 Float3Position = ImVec2(
		FloatDescPosition.x,
		FloatDescPosition.y + FloatSizeY
	);

	SetCursorScreenPos(FloatDescPosition);
	Text("Float X");
	SetCursorScreenPos(Float3Position);
	ImGui::SetNextItemWidth(FloatSizeX);
	PushID(NodeID.c_str());
	if (InputFloat("", FloatVariableOutputPortCached->GetPointerFloatData()))
	{
		FloatVariableOutputPortCached->Connect(FloatVariableOutputPortCached->GetConnectedPort());
	}
	PopID();
}
