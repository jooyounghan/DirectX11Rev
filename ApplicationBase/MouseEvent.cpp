#include "MouseEvent.h"

void MouseEvent::AddEventHandler(const std::function<void(const float&, const float&)>& EventHandler)
{
	MouseEventHandlers.push_back(EventHandler);
}

void MouseEvent::RunEventHandlers(const float& NdcX, const float& NdcY)
{
	for (auto& MouseEventHandler : MouseEventHandlers)
	{
		MouseEventHandler(NdcX, NdcY);
	}
}
