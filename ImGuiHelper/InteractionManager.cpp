#include "InteractionManager.h"
#include <algorithm>

using namespace std;
using namespace ImGui;

void InteractionManager::RegisterInteractable(AInteractable* interactable) { m_interactables.emplace_front(interactable); }

void InteractionManager::DeregisterInteractable(AInteractable* interactable)
{
	m_interactables.erase(std::remove(m_interactables.begin(), m_interactables.end(), interactable), m_interactables.end());
}

void InteractionManager::BringInteractableToFront(AInteractable* interactable)
{
	stable_partition(m_interactables.begin(), m_interactables.end(), [&](AInteractable* i) { return i == interactable; });
}

void InteractionManager::ClearRegisteredInteractables() { m_interactables.clear(); }

void InteractionManager::CheckMouseControlEvents()
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
			if (!imGuiIO.WantCaptureMouse) return;

			if (imGuiIO.MouseClicked[idx])
			{				
				MouseClickEventArgs mouseClickEventArgs(mouseEventArgs, mouseSource, EMouseEvent::CLICKED);
				IterateInteractablesWithInnerMouseClickEvent(mouseClickEventArgs, bind(&RaiseInnerClickEvent, placeholders::_1, placeholders::_2));
			}
			if (imGuiIO.MouseDoubleClicked[idx])
			{
				MouseClickEventArgs mouseDoubleClickEventArgs(mouseEventArgs, mouseSource, EMouseEvent::DBL_CLICKED);
				IterateInteractablesWithInnerMouseClickEvent(mouseDoubleClickEventArgs, bind(&RaiseInnerDoubleClickEvent, placeholders::_1, placeholders::_2));
			}

			MouseClickEventArgs mouseDownEventArgs(mouseEventArgs, mouseSource, EMouseEvent::DOWN, imGuiIO.MouseDownDuration[idx]);
			IterateInteractablesWithInnerMouseClickEvent(mouseDownEventArgs, bind(&RaiseInnerDownEvent, placeholders::_1, placeholders::_2));
		}
		if (imGuiIO.MouseReleased[idx])
		{
			MouseClickEventArgs mouseReleasedEventArgs(mouseEventArgs, mouseSource, EMouseEvent::RELEASED);
			IterateInteractablesWithInnerMouseClickEvent(mouseReleasedEventArgs, bind(&RaiseInnerUpEvent, placeholders::_1, placeholders::_2));
			IterateInteractablesWithMouseClickEvent(mouseReleasedEventArgs, bind(&RaiseDragEndEvent, placeholders::_1, placeholders::_2));
		}
	}

}

void InteractionManager::IterateInteractablesWithMouseEvent(MouseEventArgs& mouseEventArgs)
{
	for (AInteractable* const interactable : m_interactables)
	{
		if (interactable->IsMousePressed())
		{
			interactable->OnDragging(mouseEventArgs);
		}

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

void InteractionManager::IterateInteractablesWithInnerMouseClickEvent(
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

void InteractionManager::IterateInteractablesWithMouseClickEvent(
	MouseClickEventArgs& mouseClickedEventArgs, 
	const std::function<void(AInteractable* const, MouseClickEventArgs&)>& eventHandler
)
{
	for (AInteractable* const interactable : m_interactables)
	{
		eventHandler(interactable, mouseClickedEventArgs);
		if (mouseClickedEventArgs.m_isHandled)
		{
			break;
		}
	}
}


void InteractionManager::RaiseInnerClickEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	interactable->SetFocused(true);
	return interactable->OnMouseClicked(args);
}

void InteractionManager::RaiseInnerDoubleClickEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	return interactable->OnMouseDoubleClicked(args);
}

void InteractionManager::RaiseInnerDownEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	if (interactable->IsFocused())
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
}

void InteractionManager::RaiseInnerUpEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	return interactable->OnMouseUp(args);
}

void InteractionManager::RaiseDragEndEvent(AInteractable* const interactable, MouseClickEventArgs& args)
{
	if (interactable->IsMousePressed())
	{
		interactable->SetMousePressed(false);
		interactable->OnEndDrag();
	}

	if (interactable->IsFocused())
	{
		interactable->SetFocused(false);
	}
}
