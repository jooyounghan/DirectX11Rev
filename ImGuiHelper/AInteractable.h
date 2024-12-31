#pragma once
#include "imgui.h"
#include "MouseEventArgs.h"
#include "MouseClickEventArgs.h"
#include "ZIndexable.h"

class AInteractable : public ZIndexable
{
protected:
	ImVec2 m_leftTop;

protected:
	bool m_isMouseEntered = false;
	bool m_isMousePressed = false;

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

protected:
	virtual void AdjustPosition() = 0;
};

