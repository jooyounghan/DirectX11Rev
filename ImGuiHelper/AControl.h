#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MouseEventArgs.h"
#include "MouseClickEventArgs.h"

#include <d3d11.h>
#include <string>
#include <functional>

class AControl
{
protected:
	int m_zIndex = 0;

private:
	bool m_isMouseEntered = false;
	bool m_isMousePressed = false;

protected:
	float m_left;
	float m_top;

protected:
	void* m_dragSource = nullptr;

public:
	void RenderControl();

private:
	virtual void RenderControlImpl() = 0;

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const = 0;
	inline const bool& IsMouseIn() { return m_isMouseEntered; }
	inline void SetMouseIn(const bool& isIn) { m_isMouseEntered = isIn; }

public:
	inline const bool& IsMousePressed() { return m_isMousePressed; }
	inline void SetMousePressed(const bool& isPressed) { m_isMousePressed = isPressed; }

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
	virtual void OnBeginDrag() {};

public:
	inline const int& GetZIndex() { return m_zIndex; }
};

