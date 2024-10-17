#pragma once
#include "PortElement.h"

class InputPort;

class AOutputPort : public PortElement
{
public:
	AOutputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn,
		const ImU32& BasePortColorIn,
		const ImU32& HilightedPortColorIn,
		const ImU32& ConnectionLineColorIn
	);
	virtual ~AOutputPort();

protected:
	ImU32 ConnectionLineColor;

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
	void AddConnectionLine(const ImVec2& OriginPosition, const ImVec2& ToPosition, ImDrawList* DrawListIn);

protected:
	InputPort* ConnectedPort = nullptr;

public:
	virtual void Connect(InputPort* PortIn);

protected:
	virtual bool IsConnectable(InputPort* PortIn) = 0;

private:
	bool IsCyclic(InputPort* TartgetPort);
	bool IsCyclic(NodeElement* InitialCheckNode);
};

