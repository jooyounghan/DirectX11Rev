#pragma once
#include "AInteractable.h"
#include "ZIndexComparer.h"

#include <set>
#include <functional>

class ImGuiInteractionManager
{
public:
	ImGuiInteractionManager() = default;
    virtual ~ImGuiInteractionManager() = default;

protected:
    std::set<AInteractable*, ZIndexComparer> m_interactables;

public:
    inline void RegisterInteractable(AInteractable* interactable) { m_interactables.insert(interactable); }
    inline void DeregisterInteractable(AInteractable* interactable) { m_interactables.erase(interactable); }
    inline void ClearRegisteredInteractables() { m_interactables.clear(); }
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

