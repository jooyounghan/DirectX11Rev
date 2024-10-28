#include "Float3InputNode.h"
#include "Float3VariableOutputPort.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

Float3InputNode::Float3InputNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn,
	const XMFLOAT3& Float3DataIn
)
	: VariableInputNode(CenterPositionIn, RectangleSizeIn),
	Float3SizeX(ElementSize.x - 2.f * NodePaddingSize),
	Float3SizeY((ElementSize.y - 2.f * NodePaddingSize) / 2.f)
{
	Float3VariableOutputPortCached = AddOutputPort<Float3VariableOutputPort>(ImVec2(NULL, NULL), 10.f);
	Float3VariableOutputPortCached->SetFloat3Data(Float3DataIn);
}

Float3InputNode::~Float3InputNode()
{
}

void Float3InputNode::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	NodeElement::AddToDrawList(OriginPosition, DrawListIn);

	const ImVec2 Float3DescPosition = ImVec2(
		LeftTopPosition.x + OriginPosition.x + NodePaddingSize,
		LeftTopPosition.y + OriginPosition.y + NodePaddingSize
	);
	const ImVec2 Float3Position = ImVec2(
		Float3DescPosition.x,
		Float3DescPosition.y + Float3SizeY
	);



	SetCursorScreenPos(Float3DescPosition);
	Text("XYZ(RGB)");
	SetCursorScreenPos(Float3Position);
	ImGui::SetNextItemWidth(Float3SizeX);
	PushID(NodeID.c_str());
	if (InputFloat3("", &Float3VariableOutputPortCached->GetPointerFloat3Data()->x))
	{
		Float3VariableOutputPortCached->Connect(Float3VariableOutputPortCached->GetConnectedPort());
	}
	PopID();
}
