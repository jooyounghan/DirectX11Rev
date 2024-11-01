#pragma once
#include "VariableOutputPort.h"
#include "directxmath/DirectXMath.h"
#include <string>

class FloatVariableOutputPort : public VariableOutputPort
{
public:
	FloatVariableOutputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn
	);
	virtual ~FloatVariableOutputPort();

public:
	static std::string FloatVariableKind;

protected:
	float FloatData = 0.f;
	MakeSetterGetter(FloatData);
	MakePointerGetter(FloatData);

public:
	virtual bool IsConnectable(InputPort* PortIn) override;
};

