#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "Delegation.h"
#include "HeaderHelper.h"

class ADrawElement;

typedef std::function<void(ADrawElement*)>			ClickedElementDelegation;
typedef std::function<void(ADrawElement*)>			MouseDownElementDelegation;
typedef std::function<void(ADrawElement*)>			MouseUpElementDelegation;
typedef std::function<void(ADrawElement*)>			MouseEnterElementDelegation;
typedef std::function<void(ADrawElement*)>			MouseLeaveElementDelegation;

class ADrawElement
{
public:
	ADrawElement(const ImU32& BasePortColorIn, const ImU32& HilightedPortColorIn);
	virtual ~ADrawElement();

protected:
	ImU32 BaseColor;
	ImU32 HilightedColor;

public:
	Delegation<ADrawElement*>	ClickedEvent;

	Delegation<ADrawElement*>	MouseDownEvent;
	Delegation<ADrawElement*>	MouseUpEvent;

	Delegation<ADrawElement*>	MouseEnterEvent;
	Delegation<ADrawElement*>	MouseLeaveEvent;

protected:
	bool bIsElementFocused = false;
	bool bIsMouseEntered = false;

public:
	virtual void SetFocus(const bool& IsFocused);

public:
	virtual const ImVec2& GetPosition() = 0;
	virtual void SetPosition(const ImVec2& CenterPositionIn) = 0;
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) = 0;
};