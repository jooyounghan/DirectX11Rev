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
	: AApplication(), m_engine(D3D11Engine::GetInstance()), m_taskManager(TaskManager::GetInstance())
{
	m_defferedContexts[EDefferedContextType::ASSETS] = new DefferedContext(m_engine->GetDeviceAddress());
	m_defferedContexts[EDefferedContextType::WINDOWS] = new DefferedContext(m_engine->GetDeviceAddress());

	m_assetManager = new AssetManager();

	m_onWindowSizeMoveHandler = [&](const UINT& widthIn, const UINT& heightIn) { m_engine->ResizeSwapChain(widthIn, heightIn); };
	m_imguiWindows.emplace_back(new AssetViewWindow("AssetManager", m_assetManager));

	TaskModal* taskModal = new TaskModal("Processing...");
	m_imguiModals.emplace_back(taskModal);
	m_taskManager->OnTaskStarted = bind(&TaskModal::SetTaskDescription, taskModal, placeholders::_1, placeholders::_2);
	m_taskManager->OnTasksCompleted = bind(&TaskModal::SetTasksCompleted, taskModal);

}

YHEngine::GameEngine::~GameEngine()
{
	m_taskManager->FinishLaunchingTasks();

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

	m_engine->InitEngine(m_appSize.m_width, m_appSize.m_height, m_mainWindow);
	for (auto& defferedContexts : m_defferedContexts)
	{
		defferedContexts.second->InitDefferedContext();
	}

	ID3D11Device* device = m_engine->GetDevice();
	ID3D11DeviceContext* deviceContext = m_engine->GetDeviceContext();

	ImGuiInitializer::InitImGui(m_mainWindow, device, deviceContext);

	OnWindowSizeMove();

	m_assetManager->RegisterAssetReadPath("./Assets");
	m_assetManager->RegisterAssetWritePath("./Assets");

	m_taskManager->StartLaunchingTasks();

	m_taskManager->RegisterTask([&]() { m_assetManager->PreloadFromResources(); }, "Load Asset From Resources...");

	DefferedContext* assetsDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS];
	DefferedContext* windowsDefferedContext = m_defferedContexts[EDefferedContextType::WINDOWS];

	for (auto& imguiWindow : m_imguiWindows)
	{
		m_taskManager->RegisterTask([&, device, windowsDefferedContext]()
			{ 
				imguiWindow->InitializeWindow(device, windowsDefferedContext->GetDefferedContext());
				windowsDefferedContext->RecordToCommandList();
			}, "Initializing Windows..."
		);
	}

	m_taskManager->RegisterTask([&, device, assetsDefferedContext]()
		{ 
			m_assetManager->PreloadFromDirectories(device, assetsDefferedContext->GetDefferedContext());
			assetsDefferedContext->RecordToCommandList();
		}, "Load Asset From Directories..."
	);
}

void GameEngine::Update(const float& deltaTime)
{
	m_engine->ClearBackBuffer(ClearColor);
	m_engine->SetRTVAsBackBuffer();

	ID3D11DeviceContext* immediateContext = m_engine->GetDeviceContext();

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

	m_engine->GetSwapChain()->Present(1, 0);
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
			ID3D11Device* device = m_engine->GetDevice();
			DefferedContext* assetsDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS];

			string filePathStr = string(filePath);

			m_taskManager->RegisterTask([&, device, assetsDefferedContext, filePathStr]()
				{
					m_assetManager->WrtieFileAsAsset(device, assetsDefferedContext->GetDefferedContext(), filePathStr);
					assetsDefferedContext->RecordToCommandList();
				}, "Load Asset From Files..."
			);
		}
	}
	DragFinish(hDrop);
}
