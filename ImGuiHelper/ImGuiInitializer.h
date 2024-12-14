#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace ImGuiInitializer
{
	void InitImGui(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
}