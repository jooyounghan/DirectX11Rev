#pragma once
#include "AInteractable.h"

class ADrawElement : public AInteractable
{
public:
	ADrawElement(const ImVec2& leftTop);

public:
	void Draw(ImDrawList* drawListIn);

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) = 0;
};

