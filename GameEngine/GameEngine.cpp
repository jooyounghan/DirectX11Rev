#include "GameEngine.h"
#include "ImGuiInitializer.h"

#include "AssetViewWindow.h"
#include "TaskModal.h"

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

	m_defferedContexts[EDefferedContextType::ASSETS] = new DefferedContext(engine->GetDeviceAddress());
	m_defferedContexts[EDefferedContextType::WINDOWS] = new DefferedContext(engine->GetDeviceAddress());

	m_assetManager = new AssetManager();

	m_onWindowSizeMoveHandler = [&](const UINT& widthIn, const UINT& heightIn) { GetEngine()->ResizeSwapChain(widthIn, heightIn); };
	m_imguiWindows.emplace_back(new AssetViewWindow("AssetManager", m_assetManager));

	TaskModal* taskModal = new TaskModal("Processing...");
	m_imguiModals.emplace_back(taskModal);
	m_taskManager.OnTaskStarted = bind(&TaskModal::SetTaskDescription, taskModal, placeholders::_1, placeholders::_2);
	m_taskManager.OnTasksCompleted = bind(&TaskModal::SetTasksCompleted, taskModal);

}

YHEngine::GameEngine::~GameEngine()
{
	m_taskManager.FinishLaunchingTasks();

	delete m_assetManager;

	for (auto& defferedContexts : m_defferedContexts)
	{
		delete defferedContexts.second;
	}

	for (AWindow* imguiWindow : m_imguiWindows)
	{
		delete imguiWindow;
	}

	for (AModal* imguiModal : m_imguiModals)
	{
		delete imguiModal;
	}
}

void GameEngine::Init(const wchar_t* className, const wchar_t* applicaitonName)
{
	AApplication::Init(className, applicaitonName);

	D3D11Engine* engine = GetEngine();
	if (engine != nullptr)
	{
		engine->InitEngine(m_appSize.width, m_appSize.height, m_mainWindow);
		for (auto& defferedContexts : m_defferedContexts)
		{
			defferedContexts.second->InitDefferedContext();
		}
	}

	ID3D11Device* device = engine->GetDevice();
	ID3D11DeviceContext* deviceContext = engine->GetDeviceContext();

	ImGuiInitializer::InitImGui(m_mainWindow, device, deviceContext);

	OnWindowSizeMove();

	m_assetManager->RegisterAssetReadPath("./Assets");
	m_assetManager->RegisterAssetWritePath("./Assets");

	m_taskManager.StartLaunchingTasks();

	m_taskManager.RegisterTask([&]() { m_assetManager->PreloadFromResources(); }, "Load Asset From Resources...");

	DefferedContext* assetsDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS];
	DefferedContext* windowsDefferedContext = m_defferedContexts[EDefferedContextType::WINDOWS];

	for (auto& imguiWindow : m_imguiWindows)
	{
		m_taskManager.RegisterTask([&, device, windowsDefferedContext]()
			{ 
				imguiWindow->InitializeWindow(device, windowsDefferedContext->GetDefferedContext());
				windowsDefferedContext->RecordToCommandList();
			}, "Initializing Windows..."
		);
	}

	m_taskManager.RegisterTask([&, device, assetsDefferedContext]()
		{ 
			m_assetManager->PreloadFromDirectories(device, assetsDefferedContext->GetDefferedContext());
			assetsDefferedContext->RecordToCommandList();
		}, "Load Asset From Directories..."
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

	ID3D11DeviceContext* immediateContext = engine->GetDeviceContext();

	for (auto& defferedContexts : m_defferedContexts)
	{
		defferedContexts.second->TryExecuteCommandList(immediateContext);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	for (auto& imguiWindow : m_imguiWindows)
	{
		imguiWindow->ShowWindow();
	}

	for (auto& imguiModal : m_imguiModals)
	{
		imguiModal->DoModal();
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
			D3D11Engine* engine = GetEngine();
			ID3D11Device* device = engine->GetDevice();
			DefferedContext* assetsDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS];

			string filePathStr = string(filePath);

			m_taskManager.RegisterTask([&, device, assetsDefferedContext, filePathStr]()
				{
					m_assetManager->WrtieFileAsAsset(device, assetsDefferedContext->GetDefferedContext(), filePathStr);
					assetsDefferedContext->RecordToCommandList();
				}, "Load Asset From Files..."
			);
		}
	}
	DragFinish(hDrop);
}
