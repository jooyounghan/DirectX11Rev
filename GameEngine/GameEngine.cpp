#include "GameEngine.h"
#include "ImGuiInitializer.h"

#include "AssetViewWindow.h"
#include "SceneWindow.h"

#include "TaskModal.h"
#include "MessageBoxModal.h"

#include <future>

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
	: AApplication(), 
	m_engine(D3D11Engine::GetInstance()), 
	m_taskManager(TaskManager::GetInstance())
{
	ID3D11Device** deviceAddress = m_engine->GetDeviceAddress();
	ID3D11DeviceContext** deviceContextAddress = m_engine->GetDeviceContextAddress();


	/* Deffered Context */
	m_defferedContexts[EDefferedContextType::ASSETS_LOAD] = new DefferedContext(deviceAddress);
	m_defferedContexts[EDefferedContextType::COMPONENT_UPDATE] = new DefferedContext(deviceAddress);
	m_defferedContexts[EDefferedContextType::COMPONENT_RENDER] = new DefferedContext(deviceAddress);

	/* Session Manager */
	m_sessionManager = new SessionManager("localhost", 33060, "gameEngineSession", "YHengine12!@");

	/* Asset Manager*/
	m_assetManager = new AssetManager();
	m_assetManager->RegisterAssetReadPath("./Assets");
	m_assetManager->RegisterAssetWritePath("./Assets");

	/* Component Manager*/
	m_componentManager = new ComponentManager(m_sessionManager, m_assetManager, deviceAddress, deviceContextAddress);

	/* PSO Manager */
	m_psoManager = new PSOManager(deviceAddress);

	/* Window */
	m_imguiWindows.emplace_back(new AssetViewWindow("AssetManager", m_assetManager));
	m_imguiWindows.emplace_back(new SceneWindow(
		"Scene", m_defferedContexts[EDefferedContextType::COMPONENT_RENDER]->GetDefferedContextAddress(), 
		m_assetManager, m_componentManager, nullptr/* PSOManager */
	));

	/* Modal */
	TaskModal* taskModal = new TaskModal("Processing...");
	MessageBoxModal* errorMessageBoxModal = new MessageBoxModal("Error!");
	m_imguiModals.emplace_back(taskModal);
	m_imguiModals.emplace_back(errorMessageBoxModal);

	/* Bind Event Handler */
	m_onWindowSizeMoveHandler = [&](const UINT& widthIn, const UINT& heightIn) { m_engine->ResizeSwapChain(widthIn, heightIn); };
	m_taskManager->OnTaskInserted = bind(&TaskModal::SetTaskCount, taskModal, placeholders::_1);
	m_taskManager->OnTaskStarted = bind(&TaskModal::SetTaskDescription, taskModal, placeholders::_1, placeholders::_2);
	m_taskManager->OnTasksCompleted = bind(&TaskModal::SetTasksCompleted, taskModal);
	m_componentManager->OnErrorOccurs = bind(&MessageBoxModal::ModalMessage, errorMessageBoxModal, placeholders::_1);
}

YHEngine::GameEngine::~GameEngine()
{
	m_taskManager->FinishLaunchingTasks();

	delete m_sessionManager;
	delete m_assetManager;
	delete m_componentManager;

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

	m_taskManager->StartLaunchingTasks();

	DefferedContext* assetsLoadDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS_LOAD];

	m_taskManager->RegisterTask([&, device, assetsLoadDefferedContext]() 
		{ 
			m_assetManager->PreloadFromResources(device, assetsLoadDefferedContext->GetDefferedContext());
		}, "Load Asset From Resources..."
	);

	m_taskManager->RegisterTask([&, device, assetsLoadDefferedContext]()
		{ 
			m_assetManager->PreloadFromDirectories(device, assetsLoadDefferedContext->GetDefferedContext());
		}, "Load Asset From Directories..."
	);

	m_taskManager->RegisterTask([&, assetsLoadDefferedContext]()
		{
			assetsLoadDefferedContext->RecordToCommandList();
		}, "Load Asset From Directories..."
	);

	m_taskManager->RegisterTask([&]()
		{
			m_componentManager->InitComponentManager();
		}, "Load Component From DB..."
	);
}

void GameEngine::Update(const float& deltaTime)
{
	m_engine->ClearBackBuffer(ClearColor);
	m_engine->SetRTVAsBackBuffer();

	ID3D11DeviceContext* immediateContext = m_engine->GetDeviceContext();

	future<void> assetLoadGPUTask = async(launch::deferred, [&, immediateContext]() 
		{
			DefferedContext* assetLoadDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS_LOAD];
			assetLoadDefferedContext->TryExecuteCommandList(immediateContext); 
		});

	future<void> componentUpdateGPUTask = async(launch::deferred, [&, immediateContext]() 
		{
			DefferedContext* componentUpdateDefferedContext = m_defferedContexts[EDefferedContextType::COMPONENT_UPDATE];
			componentUpdateDefferedContext->TryExecuteCommandList(immediateContext);
		});

	for (auto& imguiWindow : m_imguiWindows)
	{
		imguiWindow->PrepareWindow();
	}

	future<void> componentRenderGPUTask = async(launch::deferred, [&, immediateContext]()
		{
			DefferedContext* componentRenderDefferedContext = m_defferedContexts[EDefferedContextType::COMPONENT_RENDER];
			componentRenderDefferedContext->TryExecuteCommandList(immediateContext);
		});

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

	assetLoadGPUTask.wait();
	componentUpdateGPUTask.wait();
	componentRenderGPUTask.wait();

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
			DefferedContext* assetsDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS_LOAD];

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
