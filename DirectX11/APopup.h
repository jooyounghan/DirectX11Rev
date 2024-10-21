#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "Delegation.h"

#include <string>

class APopup
{
public:
	APopup(const std::string& PopupIDIn);
	virtual ~APopup();

protected:
	std::string PopupID;

public:
	void PopUp(const ImVec2& MinRectIn, const ImVec2& MaxRectIn);

protected:
	virtual void RenderPopupMenu() = 0;
};

