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
	ImVec2 LeftTopPosition;
	ImVec2 RightBottomPosition;
	ImVec2 ElementSize;

public:
	virtual const ImVec2& GetPosition() override;

public:
	virtual void SetPosition(const ImVec2& CenterPositionIn) override;
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
};

