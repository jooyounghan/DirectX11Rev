#include "KeyEvent.h"

void KeyEvent::AddEventHandler(const std::function<void(const float&)>& EventHandler)
{
	KeyEventHandlers.push_back(EventHandler);
}

void KeyEvent::RunEventHandlers(const float& DeltaTime)
{
	for (auto& KeyEventHandler : KeyEventHandlers)
	{
		KeyEventHandler(DeltaTime);
	}
}
