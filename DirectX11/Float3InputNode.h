#pragma once
#include "VariableInputNode.h"
#include "directxmath/DirectXMath.h"

class Float3VariableOutputPort;

class Float3InputNode : public VariableInputNode
{
public:
	Float3InputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const DirectX::XMFLOAT3& Float3DataIn
	);
	virtual ~Float3InputNode();

protected:
	Float3VariableOutputPort* Float3VariableOutputPortCached = nullptr;
	MakeGetter(Float3VariableOutputPortCached);

protected:
	const float Float3SizeX;
	const float Float3SizeY;

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
};

