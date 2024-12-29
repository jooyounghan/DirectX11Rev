#include "ImGuiControlManager.h"
#include "MouseEventArgs.h"
#include "MouseClickEventArgs.h"

using namespace std;
using namespace ImGui;

void ImGuiControlManager::CheckMouseControlEvents()
{
	ImGuiIO& imGuiIO = ImGui::GetIO();

	MouseEventArgs mouseEventArgs(
		imGuiIO.MousePos.x, imGuiIO.MousePos.y,
		imGuiIO.MouseDelta.x, imGuiIO.MouseDelta.y
	);
	IterateControlWithMouseEvent(mouseEventArgs);

	for (size_t idx = 0; idx < 3; ++idx)
	{
		EMouseSource mouseSource = EMouseSource::NONE;
		switch (idx)
		{
		case 0:
			mouseSource = EMouseSource::LEFT_BUTTON;
			break;
		case 1:
			mouseSource = EMouseSource::RIGHT_BUTTON;
			break;
		case 2:
			mouseSource = EMouseSource::MIDDLE_BUTTON;
			break;
		default:
			break;
		}

		if (imGuiIO.MouseDown[idx])
		{
			if (imGuiIO.MouseClicked[idx])
			{				
				MouseClickEventArgs mouseClickEventArgs(mouseEventArgs, mouseSource, EMouseEvent::CLICKED);
				IterateControlsWithMouseClickEvent(mouseClickEventArgs, bind(&RaiseClickEvent, placeholders::_1, placeholders::_2));
			}
			if (imGuiIO.MouseDoubleClicked[idx])
			{
				MouseClickEventArgs mouseDoubleClickEventArgs(mouseEventArgs, mouseSource, EMouseEvent::DBL_CLICKED);
				IterateControlsWithMouseClickEvent(mouseDoubleClickEventArgs, bind(&RaiseDoubleClickEvent, placeholders::_1, placeholders::_2));
			}

			MouseClickEventArgs mouseDownEventArgs(mouseEventArgs, mouseSource, EMouseEvent::DOWN, imGuiIO.MouseDownDuration[idx]);
			IterateControlsWithMouseClickEvent(mouseDownEventArgs, bind(&RaiseDownEvent, placeholders::_1, placeholders::_2));
		}
		if (imGuiIO.MouseReleased[idx])
		{
			MouseClickEventArgs mouseReleasedEventArgs(mouseEventArgs, mouseSource, EMouseEvent::RELEASED);
			IterateControlsWithMouseClickEvent(mouseReleasedEventArgs, bind(&RaiseReleasedEvent, placeholders::_1, placeholders::_2));
		}
	}

}

void ImGuiControlManager::IterateControlWithMouseEvent(MouseEventArgs& mouseEventArgs)
{
	for (AUserControl* const control : m_controls)
	{
		if (control->IsPointIn(mouseEventArgs.m_mousePosX, mouseEventArgs.m_mousePosY))
		{
			if (control->IsMouseIn())
			{
				control->OnMouseHovering(mouseEventArgs);
			}
			else
			{
				control->SetMouseIn(true);
				control->OnMouseEnter(mouseEventArgs);
			}
		}
		else
		{
			if (control->IsMouseIn())
			{
				control->SetMouseIn(false);
				control->OnMouseLeave(mouseEventArgs);
			}
		}
	}
}

void ImGuiControlManager::IterateControlsWithMouseClickEvent(
	MouseClickEventArgs& mouseClickedEventArgs, 
	const std::function<void(AUserControl* const, MouseClickEventArgs&)>& eventHandler
)
{
	for (AUserControl* const control : m_controls)
	{
		if (control->IsPointIn(mouseClickedEventArgs.m_mousePosX, mouseClickedEventArgs.m_mousePosY))
		{
			eventHandler(control, mouseClickedEventArgs);
			if (mouseClickedEventArgs.m_isHandled)
			{
				break;
			}
		}
	}
}

void ImGuiControlManager::RaiseClickEvent(AUserControl* const control, MouseClickEventArgs& args)
{
	return control->OnMouseClicked(args);
}

void ImGuiControlManager::RaiseDoubleClickEvent(AUserControl* const control, MouseClickEventArgs& args)
{
	return control->OnMouseDoubleClicked(args);
}

void ImGuiControlManager::RaiseDownEvent(AUserControl* const control, MouseClickEventArgs& args)
{
	if (control->IsMousePressed())
	{
		return control->OnMouseDown(args);
	}
	else
	{
		control->SetMousePressed(true);
		return control->OnBeginDrag();
	}

}

void ImGuiControlManager::RaiseReleasedEvent(AUserControl* const control, MouseClickEventArgs& args)
{
	control->SetMousePressed(false);
	return control->OnMouseReleased(args);
}
