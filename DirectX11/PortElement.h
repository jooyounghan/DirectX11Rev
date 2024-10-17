#pragma once
#include "ADrawElement.h"

class NodeElement;

class PortElement : public ADrawElement
{
public:
	PortElement(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn,
		const ImU32& BasePortColorIn,
		const ImU32& HilightedPortColorIn
	);
	virtual ~PortElement();

protected:
	ImVec2 CenterPosition;
	float RadiusSize;

protected:
	bool bIsConnecting = false;

protected:
	NodeElement* ParentNodeElement;
	MakeSetterGetter(ParentNodeElement);

public:
	inline void SetIsConnecting(const bool& IsConnecting) { bIsConnecting = IsConnecting; }

public:
	virtual const ImVec2& GetPosition() override;
	virtual void SetPosition(const ImVec2& CenterPositionIn) override;
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;

public:
	bool IsHoveringOnPort(const ImVec2& OriginPosition);
};

