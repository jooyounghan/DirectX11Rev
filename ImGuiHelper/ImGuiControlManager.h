#pragma once
#include "AUserControl.h"
#include <set>

class ControlZComparer
{
public:
    bool operator()(AUserControl* control1, AUserControl* control2) const
    {
        if (control1->GetZIndex() != control2->GetZIndex())
        {
            return control1->GetZIndex() > control2->GetZIndex();
        }
        return control1 < control2;
    }
};


class ImGuiControlManager
{
public:
	ImGuiControlManager() = default;

protected:
    std::set<AUserControl*, ControlZComparer> m_controls;

public:
    inline void RegisterControl(AUserControl* control) { m_controls.insert(control); }
    inline void DeregisterControl(AUserControl* control) { m_controls.erase(control); }
    inline void ClearRegisteredControl() { m_controls.clear(); }
    void CheckMouseControlEvents();

private:
    void IterateControlWithMouseEvent(MouseEventArgs& mouseEventArgs);
    void IterateControlsWithMouseClickEvent(
        MouseClickEventArgs& mouseClickedEventArgs, 
        const std::function<void(AUserControl* const, MouseClickEventArgs&)>& checker
    );

private:
    static void RaiseClickEvent(AUserControl* const control, MouseClickEventArgs& args);
    static void RaiseDoubleClickEvent(AUserControl* const control, MouseClickEventArgs& args);
    static void RaiseDownEvent(AUserControl* const control, MouseClickEventArgs& args);
    static void RaiseReleasedEvent(AUserControl* const control, MouseClickEventArgs& args);
};

