#pragma once
#include "AOutputPort.h"

class VariableOutputPort : public AOutputPort
{
public:
	VariableOutputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn
	);
	virtual ~VariableOutputPort();

public:
	virtual bool IsConnectable(InputPort* PortIn) override;
};

