#include "ImGuiControlManager.h"
#include "MouseEventArgs.h"
#include "MouseClickEventArgs.h"

using namespace std;
using namespace ImGui;

void ImGuiInteractionManager::CheckMouseControlEvents()
{
	ImGuiIO& imGuiIO = ImGui::GetIO();

	MouseEventArgs mouseEventArgs(
		imGuiIO.MousePos.x, imGuiIO.MousePos.y,
		imGuiIO.MouseDelta.x, imGuiIO.MouseDelta.y
	);
	IterateInteractablesWithMouseEvent(mouseEventArgs);

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
				IterateInteractablesWithMouseClickEvent(mouseClickEventArgs, bind(&RaiseClickEvent, placeholders::_1, placeholders::_2));
			}
			if (imGuiIO.MouseDoubleClicked[idx])
			{
				MouseClickEventArgs mouseDoubleClickEventArgs(mouseEventArgs, mouseSource, EMouseEvent::DBL_CLICKED);
				IterateInteractablesWithMouseClickEvent(mouseDoubleClickEventArgs, bind(&RaiseDoubleClickEvent, placeholders::_1, placeholders::_2));
			}

			MouseClickEventArgs mouseDownEventArgs(mouseEventArgs, mouseSource, EMouseEvent::DOWN, imGuiIO.MouseDownDuration[idx]);
			IterateInteractablesWithMouseClickEvent(mouseDownEventArgs, bind(&RaiseDownEvent, placeholders::_1, placeholders::_2));
		}
		if (imGuiIO.MouseReleased[idx])
		{
			MouseClickEventArgs mouseReleasedEventArgs(mouseEventArgs, mouseSource, EMouseEvent::RELEASED);
			IterateInteractablesWithMouseClickEvent(mouseReleasedEventArgs, bind(&RaiseReleasedEvent, placeholders::_1, placeholders::_2));
		}
	}

}

void ImGuiInteractionManager::IterateInteractablesWithMouseEvent(MouseEventArgs& mouseEventArgs)
{
	for (AInteractable* const interactable : m_interactables)
	{
		if (interactable->IsPointIn(mouseEventArgs.m_mousePosX, mouseEventArgs.m_mousePosY))
		{
			if (interactable->IsMouseIn())
			{
				interactable->OnMouseHovering(mouseEventArgs);
			}
			else
			{
				interactable->SetMouseIn(true);
				interactable->OnMouseEnter(mouseEventArgs);
			}
		}
		else
		{
			if (interactable->IsMouseIn())
			{
				interactable->SetMouseIn(false);
				interactable->OnMouseLeave(mouseEventArgs);
			}
		}
	}
}

void ImGuiInteractionManager::IterateInteractablesWithMouseClickEvent(
	MouseClickEventArgs& mouseClickedEventArgs, 
	const std::function<void(AInteractable* const, MouseClickEventArgs&)>& eventHandler
)
{
	for (AInteractable* const interactable : m_interactables)
	{
		if (interactable->IsPointIn(mouseClickedEventArgs.m_mousePosX, mouseClickedEventArgs.m_mousePosY))
		{
			eventHandler(interactable, mouseClickedEventArgs);
			if (mouseClickedEventArgs.m_isHandled)
			{
				break;
			}
		}
	}
}

void ImGuiInteractionManager::RaiseClickEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	return interactable->OnMouseClicked(args);
}

void ImGuiInteractionManager::RaiseDoubleClickEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	return interactable->OnMouseDoubleClicked(args);
}

void ImGuiInteractionManager::RaiseDownEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	if (interactable->IsMousePressed())
	{
		return interactable->OnMouseDown(args);
	}
	else
	{
		interactable->SetMousePressed(true);
		return interactable->OnBeginDrag();
	}

}

void ImGuiInteractionManager::RaiseReleasedEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	interactable->SetMousePressed(false);
	return interactable->OnMouseReleased(args);
}
