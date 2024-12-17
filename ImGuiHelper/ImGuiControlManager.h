#pragma once
#include "AControl.h"
#include <set>

class ControlZComparer
{
public:
    bool operator()(AControl* control1, AControl* control2) const
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
    std::set<AControl*, ControlZComparer> m_controls;

public:
    inline void RegisterControl(AControl* control) { m_controls.insert(control); }
    inline void DeregisterControl(AControl* control) { m_controls.erase(control); }
    inline void ClearRegisteredControl() { m_controls.clear(); }
    void CheckMouseControlEvents();

private:
    void IterateControlWithMouseEvent(MouseEventArgs& mouseEventArgs);
    void IterateControlsWithMouseClickEvent(
        MouseClickEventArgs& mouseClickedEventArgs, 
        const std::function<void(AControl* const, MouseClickEventArgs&)>& checker
    );

private:
    static void RaiseClickEvent(AControl* const control, MouseClickEventArgs& args);
    static void RaiseDoubleClickEvent(AControl* const control, MouseClickEventArgs& args);
    static void RaiseDownEvent(AControl* const control, MouseClickEventArgs& args);
    static void RaiseReleasedEvent(AControl* const control, MouseClickEventArgs& args);
};

