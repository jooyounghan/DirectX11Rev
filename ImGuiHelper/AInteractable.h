#pragma once
#include "imgui.h"
#include "MouseEventArgs.h"
#include "MouseClickEventArgs.h"

#include <functional>

class InteractionManager;

class AInteractable
{
public:
	virtual ~AInteractable() = default;

protected:
	bool m_isMouseEntered = false;
	bool m_isMousePressed = false;
	bool m_isFocused = false;

public:
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager);
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager);

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const = 0;
	inline const bool& IsMouseIn() { return m_isMouseEntered; }
	inline void SetMouseIn(const bool& isIn) { m_isMouseEntered = isIn; }

public:
	inline const bool& IsMousePressed() { return m_isMousePressed; }
	inline void SetMousePressed(const bool& isPressed) { m_isMousePressed = isPressed; }

public:
	inline const bool& IsFocused() { return m_isFocused; }
	inline void SetFocused(const bool& isFocused) { m_isFocused = isFocused; }

public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) {};
	virtual void OnMouseDoubleClicked(MouseClickEventArgs& args) {};
	virtual void OnMouseDown(MouseClickEventArgs& args) {};
	virtual void OnMouseUp(MouseClickEventArgs& args) {};

public:
	virtual void OnMouseEnter(MouseEventArgs& args) {};
	virtual void OnMouseLeave(MouseEventArgs& args) {};
	virtual void OnMouseHovering(MouseEventArgs& args) {};

public:
	virtual void OnBeginDrag() {};
	virtual void OnDragging(MouseEventArgs& args) {};
	virtual void OnEndDrag() {};

protected:
	virtual void AdjustPosition() = 0;
};

