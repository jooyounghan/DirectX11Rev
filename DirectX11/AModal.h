#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "Delegation.h"

#include <string>

class AModal
{
public:
	AModal(const std::string& ModalHeaderNameIn);
	virtual ~AModal();

protected:
	const std::string ModalHeaderName;

public:
	void DoModal();

protected:
	ImVec2 PreviousWindowPos = ImVec2();

protected:
	void SetWindowPosToCenter();
	void ResetWindowPosToPrevious();

protected:
	virtual bool ModalCondition() = 0;
	virtual void RenderModal() = 0;
};

