#include "GameEngine.h"
#include "ImGuiInitializer.h"

#include "AssetViewWindow.h"

using namespace std;
using namespace D3D11;
using namespace YHEngine;

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
	D3D11Engine* engine = GetEngine();
	m_assetManager = new AssetManager(engine->GetDeviceAddress(), engine->GetDeviceContextAddress());
	m_onWindowSizeMoveHandler = [&](const UINT& widthIn, const UINT& heightIn) { GetEngine()->ResizeSwapChain(widthIn, heightIn); };
	m_imguiWindows.emplace_back(make_unique<AssetViewWindow>("AssetManager", m_assetManager));
}

YHEngine::GameEngine::~GameEngine()
{
	delete m_assetManager;
}

void GameEngine::Init(const wchar_t* className, const wchar_t* applicaitonName)
{
	AApplication::Init(className, applicaitonName);

	D3D11Engine* engine = GetEngine();
	if (engine != nullptr)
	{
		engine->InitEngine(m_appSize.width, m_appSize.height, m_mainWindow);
	}

	ID3D11Device* device = engine->GetDevice();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();

	ImGuiInitializer::InitImGui(m_mainWindow, device, deviceContext);

	OnWindowSizeMove();

	m_assetManager->RegisterAssetReadPath("./Assets");
	m_assetManager->RegisterAssetWritePath("./Assets");
	m_assetManager->PreloadFromResources();

	for (auto& imguiWindow : m_imguiWindows)
	{
		imguiWindow->InitializeWindow(device, deviceContext);
	}

	m_assetManager->PreloadFromDirectories();
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

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	for (auto& imguiWindow : m_imguiWindows)
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

	switch (msg)
	{
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		OnDropFiles(hDrop);
		break;
	}
	default:
		break;
	}
}

void YHEngine::GameEngine::OnDropFiles(const HDROP& hDrop)
{
	UINT fileCount = DragQueryFileA(hDrop, 0xFFFFFFFF, NULL, 0);
	for (UINT i = 0; i < fileCount; ++i)
	{
		char filePath[MAX_PATH];
		if (DragQueryFileA(hDrop, i, filePath, MAX_PATH))
		{
			string filePathStr = string(filePath);
			m_assetManager->WrtieFileAsAsset(filePathStr);
		}
	}
	DragFinish(hDrop);
}
