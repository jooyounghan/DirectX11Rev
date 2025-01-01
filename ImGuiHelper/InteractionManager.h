#pragma once
#include "ZIndexComparer.h"
#include "AInteractable.h"

#include <set>
#include <functional>

class InteractionManager
{
    friend class AInteractable;

public:
	InteractionManager() = default;
    virtual ~InteractionManager() = default;

private:
    std::set<AInteractable*, ZIndexComparer> m_interactables;

private:
    inline void RegisterInteractable(AInteractable* interactable) { m_interactables.insert(interactable); }
    inline void DeregisterInteractable(AInteractable* interactable) { m_interactables.erase(interactable); }

protected:
    inline void ClearRegisteredInteractables() { m_interactables.clear(); }

protected:
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

