#pragma once
#include "PortElement.h"

class OutputPort;

class InputPort : public PortElement
{
public:
	InputPort(
		const ImVec2& CenterPositionIn,
		const ImU32& BasePortColorIn,
		const ImU32& HilightedPortColorIn
	);
	virtual ~InputPort();

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;

public:
	virtual void Connect(OutputPort* PortIn) = 0;
};

