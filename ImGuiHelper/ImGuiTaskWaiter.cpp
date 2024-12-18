#include "ImGuiTaskWaiter.h"

using namespace std;

void ImGuiTaskWaiter::UpdateWaitStatus()
{
	erase_if(
		m_modalsToTask, 
		[](const auto& modalToTask) 
		{
			if (modalToTask.second.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) 
			{
				delete modalToTask.first;
				return true;
			}
			return false;
		}
	);
}
