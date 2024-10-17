#pragma once
#include "InputPort.h"

class VariableInputPort : public InputPort
{
public:
	VariableInputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn
	);

	virtual ~VariableInputPort();
};

