#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "Delegation.h"
#include "HeaderHelper.h"
class ADrawElement;

typedef std::function<void(ADrawElement*, const ImVec2&)>	DrawElementDragDelegation;
typedef std::function<void(ADrawElement*)>					DrawElementClickedDelegation;

class ADrawElement
{
protected:
	ImVec2 LeftTopPosition;
	ImVec2 RightBottomPosition;
	ImVec2 ElementSize;
	MakeGetter(ElementSize);


public:
	void SetPosition(const ImVec2& CenterPositionIn);
	void SetPosition(const ImVec2& CenterPositionIn, const ImVec2& RectangleSizeIn);

public:
	Delegation<ADrawElement*, const ImVec2&>	DrawElementDragEvent;
	Delegation<ADrawElement*>					DrawElementClickedEvent;

protected:
	bool bIsElementFocused = false;

public:
	virtual void SetFocus(const bool& IsFocused);
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) = 0;
};