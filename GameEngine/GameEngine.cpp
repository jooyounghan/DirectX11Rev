#include "GameEngine.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Externals/imgui_impl_win32.h"
#include "Externals/imgui_impl_dx11.h"

using namespace D3D11;

Utilities::SColor GameEngine::ClearColor = Utilities::SColor(0.5f, 0.2f, 0.4f, 1.0f);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

GameEngine::GameEngine()
	: AApplication(), m_engineDoublePtr(D3D11Engine::GetInstance())
{
	m_onWindowSizeMoveHandler = [&](const UINT& widthIn, const UINT& heightIn) { GetEngine()->ResizeSwapChain(widthIn, heightIn); };
}

void GameEngine::Init(const wchar_t* className, const wchar_t* applicaitonName)
{
	AApplication::Init(className, applicaitonName);
	D3D11Engine* engine = GetEngine();
	if (engine != nullptr)
	{
		engine->InitEngine(m_appSize.width, m_appSize.height, m_mainWindow);
	}

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplWin32_Init(m_mainWindow);
	ImGui_ImplDX11_Init(
		engine->GetDevice(),
		engine->GetDeviceContext()
	);
}

void GameEngine::Update(const float& deltaTime)
{
	D3D11Engine* engine = GetEngine();

	if (engine != nullptr)
	{
		engine->ClearBackBuffer(ClearColor);
		engine->SetRTVAsBackBuffer();
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	if (!main_viewport)
		return;
	main_viewport->Size = ImVec2(m_appSize.width, m_appSize.height);
	main_viewport->WorkSize = main_viewport->Size;

	ImGui::DockSpaceOverViewport(0, main_viewport);

	for (AImGuiWindow* imguiWindow : m_imguiWindows)
	{
		imguiWindow->ShowWindow();
	}

	ImGui::Render();
	ImGui::EndFrame();
	ImGui::UpdatePlatformWindows();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	engine->GetSwapChain()->Present(1, 0);
}

void GameEngine::AppProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}
