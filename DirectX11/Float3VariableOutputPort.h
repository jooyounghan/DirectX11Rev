#pragma once
#include "VariableOutputPort.h"
#include "directxmath/DirectXMath.h"
#include <string>

class Float3VariableOutputPort : public VariableOutputPort
{
public:
	Float3VariableOutputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn
	);
	virtual ~Float3VariableOutputPort();

public:
	static std::string Float3VariableKind;

protected:
	DirectX::XMFLOAT3 Float3Data = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	MakeSetterGetter(Float3Data);
	MakePointerGetter(Float3Data);

public:
	virtual bool IsConnectable(InputPort* PortIn) override;
};

