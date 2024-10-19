#pragma once
#include "PortElement.h"

class AOutputPort;

typedef std::function<void(AOutputPort*)> ConnectedDelegation;

class InputPort : public PortElement
{
public:
	InputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn,
		const ImU32& BasePortColorIn,
		const ImU32& HilightedPortColorIn
	);
	virtual ~InputPort();

public:
	void Connect(AOutputPort* OutputPortIn);
	AOutputPort* ConnectedOutputPort = nullptr;

public:
	Delegation<AOutputPort*> ConnectEvent;

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
};

