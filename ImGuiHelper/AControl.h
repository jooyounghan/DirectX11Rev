#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MouseEventArgs.h"
#include "MouseClickEventArgs.h"

#include <string>
#include <functional>

class AControl
{
protected:
	int m_zIndex = 0;

protected:
	bool m_isMouseEntered = false;

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const = 0;
	inline const bool& IsMouseIn() { return m_isMouseEntered; }
	inline void SetMouseIn(const bool& isIn) { m_isMouseEntered = isIn; }

public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) {};
	virtual void OnMouseDoubleClicked(MouseClickEventArgs& args) {};
	virtual void OnMouseDown(MouseClickEventArgs& args) {};
	virtual void OnMouseReleased(MouseClickEventArgs& args) {};
	
public:
	virtual void OnMouseEnter(MouseEventArgs& args) {};
	virtual void OnMouseLeave(MouseEventArgs& args) {};
	virtual void OnMouseHovering(MouseEventArgs& args) {};

public:
	inline const int& GetZIndex() { return m_zIndex; }
};

