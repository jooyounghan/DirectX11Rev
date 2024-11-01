#pragma once
#include "VariableInputNode.h"
#include "directxmath/DirectXMath.h"

class FloatVariableOutputPort;

class FloatInputNode : public VariableInputNode
{
public:
	FloatInputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const float& FloatDataIn
	);
	virtual ~FloatInputNode();

protected:
	FloatVariableOutputPort* FloatVariableOutputPortCached = nullptr;
	MakeGetter(FloatVariableOutputPortCached);

protected:
	const float FloatSizeX;
	const float FloatSizeY;

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;

};

