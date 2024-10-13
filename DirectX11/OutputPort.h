#pragma once
#include "PortElement.h"

class InputPort;

class OutputPort : public PortElement
{
public:
	OutputPort(
		const ImVec2& CenterPositionIn,
		const ImU32& BasePortColorIn,
		const ImU32& HilightedPortColorIn
	);
	virtual ~OutputPort();

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
	void AddConnectionLine(const ImVec2& OriginPosition, const ImVec2& ToPosition, ImDrawList* DrawListIn);

public:
	virtual void Connect(InputPort* PortIn);
};

