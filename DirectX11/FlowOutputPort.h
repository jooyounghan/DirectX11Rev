#pragma once
#include "AOutputPort.h"

class FlowOutputPort : public AOutputPort
{
public:
	FlowOutputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn
	);
	virtual ~FlowOutputPort();

public:
	virtual bool IsConnectable(InputPort* PortIn) override;
};

