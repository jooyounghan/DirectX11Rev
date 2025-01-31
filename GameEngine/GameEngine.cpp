#include "GameEngine.h"
#include "ImGuiInitializer.h"

#include "DefferedContext.h"

#include "SceneWindow.h"
#include "AssetViewWindow.h"
#include "PerformanceAnalyzerWindow.h"

#include "AssetManager.h"
#include "TaskManager.h"
#include "ComponentPSOManager.h"
#include "SessionManager.h"
#include "ComponentManager.h"

#include "TaskModal.h"
#include "MessageBoxModal.h"

#include "CameraComponent.h"
#include "InputEventManager.h"

#include "ScreenQuad.h"

#include <future>

//TEST
#include "RenderControlOption.h"

using namespace std;
using namespace D3D11;

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
	CreateEditor();

	CreateDefferedContext();
	CreateSessionManager();
	CreateAssetManager();
	CreateComponentManager();
	CreatePSOManager();
	
	m_componentManager->MonitorComponent(m_editorCamera);

	/* Bind Event Handler */
	m_onWindowSizeMoveHandler = [&](const UINT& widthIn, const UINT& heightIn) { m_engine->ResizeSwapChain(widthIn, heightIn); };
}

GameEngine::~GameEngine()
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

	ScreenQuad* screenQuadInstance = ScreenQuad::GetInstance();
	screenQuadInstance->InitScreenQuad(device);

	ImGuiInitializer::InitImGui(m_mainWindow, device, deviceContext);
	OnWindowSizeMove();

	CreateWindows();
	CreateModals();

	m_taskManager->StartLaunchingTasks();

	m_taskManager->RegisterTask([&, device]()
		{
			m_componentPSOManager->InitComopnentPSOManager(device);
		}, "Compiling Shaders..."
	);

	DefferedContext* assetsLoadDefferedContext = m_defferedContexts[EDefferedContextType::ASSETS_LOAD];

	m_taskManager->RegisterTask([&, device, assetsLoadDefferedContext]() 
		{ 
			m_assetManager->PreloadFromResources(device, assetsLoadDefferedContext->GetDefferedContext());
			assetsLoadDefferedContext->RecordToCommandList();
		}, "Load Asset From Resources..."
	);

	m_taskManager->RegisterTask([&, device, assetsLoadDefferedContext]()
		{ 
			m_assetManager->PreloadFromDirectories(device, assetsLoadDefferedContext->GetDefferedContext());
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
	m_componentManager->UpdateComponents(deltaTime);

	// TEST =================================================
	RenderControlOption::RenderBVH.ResetSerachCount();
	RenderControlOption::RenderBVH.Traverse(m_editorCamera);
	PerformanceAnalyzer::CollisionCheckCount = RenderControlOption::RenderBVH.GetSerachCount();
	// ======================================================

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


	assetLoadGPUTask.wait();
	componentUpdateGPUTask.wait();

	for (auto& imguiWindow : m_imguiWindows)
	{
		imguiWindow->PrepareWindow();
	}

	future<void> componentRenderGPUTask = async(launch::deferred, [&, immediateContext]()
		{
			DefferedContext* componentRenderDefferedContext = m_defferedContexts[EDefferedContextType::COMPONENT_RENDER];
			componentRenderDefferedContext->RecordToCommandList();
			componentRenderDefferedContext->TryExecuteCommandList(immediateContext);
		});
	componentRenderGPUTask.wait();

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
		imguiModal->DrawNotificator();
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

void GameEngine::OnDropFiles(const HDROP& hDrop)
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

void GameEngine::CreateEditor()
{	
	m_editorCamera = new CameraComponent(
		"", TempID, DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(1.f, 1.f, 1.f),
		GDefaultWidth, GDefaultHeight, GDefaultNearZ, GDefaultFarZ, GDefaultFovAngle
	);
}

void GameEngine::CreateDefferedContext()
{
	ID3D11Device* const* deviceAddress = m_engine->GetDeviceAddress();
	m_defferedContexts[EDefferedContextType::ASSETS_LOAD] = new DefferedContext(deviceAddress);
	m_defferedContexts[EDefferedContextType::COMPONENT_UPDATE] = new DefferedContext(deviceAddress);
	m_defferedContexts[EDefferedContextType::COMPONENT_RENDER] = new DefferedContext(deviceAddress);
}

void GameEngine::CreateSessionManager()
{
	m_sessionManager = new SessionManager("localhost", 33060, "gameEngineSession", "YHengine12!@");
}

void GameEngine::CreateAssetManager()
{
	m_assetManager = AssetManager::GetInstance();
	m_assetManager->RegisterAssetReadPath("./Assets");
	m_assetManager->RegisterAssetWritePath("./Assets");
}

void GameEngine::CreateComponentManager()
{
	ID3D11Device* const* deviceAddress = m_engine->GetDeviceAddress();
	m_componentManager = new ComponentManager(
		m_sessionManager, deviceAddress, 
		m_defferedContexts[EDefferedContextType::COMPONENT_UPDATE]
	);
}

void GameEngine::CreatePSOManager()
{
	ID3D11Device* const* deviceAddress = m_engine->GetDeviceAddress();
	m_componentPSOManager = ComponentPSOManager::GetInstance();
}

void GameEngine::CreateWindows()
{
	ID3D11Device* const* deviceAddress = m_engine->GetDeviceAddress();
	ID3D11DeviceContext* const* deviceContextAddress = m_engine->GetDeviceContextAddress();

	m_imguiWindows.emplace_back(new AssetViewWindow(
		"AssetViewWindow", deviceAddress, 
		m_defferedContexts[EDefferedContextType::ASSETS_LOAD]->GetDefferedContextAddress()
	));

	SceneWindow* sceneWindow = new SceneWindow(
		"SceneWindow", deviceAddress, deviceContextAddress,
		m_defferedContexts[EDefferedContextType::COMPONENT_RENDER]->GetDefferedContextAddress(),
		m_componentManager, m_componentPSOManager
	);
	sceneWindow->SetCameraComponent(m_editorCamera);
	m_imguiWindows.emplace_back(sceneWindow);

	m_imguiWindows.emplace_back(new PerformanceAnalyzerWindow("PerformanceAnalyzerWindow"));
}

void GameEngine::CreateModals()
{
	TaskModal* taskModal = new TaskModal("Processing...");
	MessageBoxModal* errorMessageBoxModal = new MessageBoxModal("Error!");
	m_imguiModals.emplace_back(taskModal);
	m_imguiModals.emplace_back(errorMessageBoxModal);

	m_taskManager->OnTaskInserted = bind(&TaskModal::SetTaskCount, taskModal, placeholders::_1);
	m_taskManager->OnTaskStarted = bind(&TaskModal::SetTaskDescription, taskModal, placeholders::_1, placeholders::_2);
	m_taskManager->OnTasksCompleted = bind(&TaskModal::SetTasksCompleted, taskModal);
	m_componentManager->OnErrorOccurs = bind(&MessageBoxModal::SetMessage, errorMessageBoxModal, placeholders::_1);
}
