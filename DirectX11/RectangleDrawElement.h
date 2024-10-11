#pragma once
#include "ADrawElement.h"

class RectangleDrawElement : public ADrawElement
{
public:
	RectangleDrawElement(
		const ImVec2& CenterPositionIn, 
		const ImVec2& RectangleSizeIn, 
		const ImU32& RectangleColorIn,
		const ImU32& HilightedColorIn
	);
	virtual ~RectangleDrawElement();

protected:
	ImU32 RectangleColor;
	ImU32 HilightedColor;

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
};

