#pragma once
#include "InputPort.h"

class FlowInputPort : public InputPort
{
public:
	FlowInputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn
	);

	virtual ~FlowInputPort();
};

