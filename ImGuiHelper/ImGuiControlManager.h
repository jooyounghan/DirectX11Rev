#pragma once
#include "AInteractable.h"
#include "ZIndexComparer.h"

#include <set>
#include <functional>

class ImGuiInteractionManager
{
public:
	ImGuiInteractionManager() = default;

protected:
    std::set<AInteractable*, ZIndexComparer> m_interactables;

public:
    inline void RegisterInteractable(AInteractable* interactable) { m_interactables.insert(interactable); }
    inline void DeregisterInteractable(AInteractable* interactable) { m_interactables.erase(interactable); }
    inline void ClearRegisteredInteractables() { m_interactables.clear(); }
    void CheckMouseControlEvents();

private:
    void IterateInteractablesWithMouseEvent(MouseEventArgs& mouseEventArgs);
    void IterateInteractablesWithMouseClickEvent(
        MouseClickEventArgs& mouseClickedEventArgs, 
        const std::function<void(AInteractable* const, MouseClickEventArgs&)>& checker
    );

private:
    static void RaiseClickEvent(AInteractable* const interactable, MouseClickEventArgs& args);
    static void RaiseDoubleClickEvent(AInteractable* const interactable, MouseClickEventArgs& args);
    static void RaiseDownEvent(AInteractable* const interactable, MouseClickEventArgs& args);
    static void RaiseReleasedEvent(AInteractable* const interactable, MouseClickEventArgs& args);
};

