#pragma once
#include "AInteractable.h"

#include <deque>   
#include <functional>

class InteractionManager
{
    friend class AInteractable;

public:
	InteractionManager() = default;
    virtual ~InteractionManager() = default;

private:
    std::deque<AInteractable*> m_interactables;

protected:
    void RegisterInteractable(AInteractable* interactable);
    void DeregisterInteractable(AInteractable* interactable);
    void BringInteractableToFront(AInteractable* interactable);
    void ClearRegisteredInteractables();
    void CheckMouseControlEvents();

private:
    void IterateInteractablesWithMouseEvent(MouseEventArgs& mouseEventArgs);
    void IterateInteractablesWithInnerMouseClickEvent(
        MouseClickEventArgs& mouseClickedEventArgs, 
        const std::function<void(AInteractable* const, MouseClickEventArgs&)>& eventHandler
    );
        void IterateInteractablesWithMouseClickEvent(
        MouseClickEventArgs& mouseClickedEventArgs, 
        const std::function<void(AInteractable* const, MouseClickEventArgs&)>& eventHandler
    );

private:
    static void RaiseInnerClickEvent(AInteractable* const interactable, MouseClickEventArgs& args);
    static void RaiseInnerDoubleClickEvent(AInteractable* const interactable, MouseClickEventArgs& args);
    static void RaiseInnerDownEvent(AInteractable* const interactable, MouseClickEventArgs& args);
    static void RaiseInnerUpEvent(AInteractable* const interactable, MouseClickEventArgs& args);
    static void RaiseDragEndEvent(AInteractable* const interactable, MouseClickEventArgs& args);
};

