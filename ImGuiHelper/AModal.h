#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include <string>

class AModal
{
public:
	AModal(const std::string& modalHeaderName);
	virtual ~AModal();

protected:
	const std::string m_modalHeaderName;

public:
	void DoModal();

protected:
	ImVec2 m_previousWindowPos = ImVec2();

protected:
	void SetWindowPosToCenter();
	void ResetWindowPosToPrevious();

protected:
	virtual bool ModalCondition() = 0;
	virtual void RenderModal() = 0;
};

