#include "PortfolioApp.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"

#include "GraphicsPipeline.h"
#include "GameWorld.h"

using namespace std;

PortfolioApp* PortfolioApp::GApp = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return PortfolioApp::GApp->AppProc(hWnd, msg, wParam, lParam);
}

PortfolioApp::PortfolioApp()
{
	GApp = this;

	LoadMonitorInfo();

	AutoZeroMemory(WindowClass);
	WindowClass = {
		sizeof(WNDCLASSEX), CS_CLASSDC, WndProc,
		0L, 0L, GetModuleHandle(NULL), 
		NULL, NULL, NULL, 
		NULL, L"Portfolio", NULL 
	};
	RegisterClassEx(&WindowClass);

	HWND hWindow = CreateWindow(
		WindowClass.lpszClassName, L"Joo YH / Portfolio_DirectX11",
		WS_OVERLAPPEDWINDOW, 0, 0,
		App::GWidth, App::GHeight, NULL,
		NULL, WindowClass.hInstance, NULL
	);
	MainWindow = hWindow;

	ShowWindow(MainWindow, SW_SHOW);

	GraphicsPipelineInstance = make_unique<GraphicsPipeline>();
	GraphicsPipelineInstance->LoadPipeline(App::GWidth, App::GHeight, hWindow);

	OnChangeWindow();

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&PrevTime);

#ifdef _DEBUG
	GameWorldInstance = make_unique<GameWorld>(GraphicsPipelineInstance.get(), MainWindow);
#else
	GameWorldInstance = make_unique<GameWorld>(GraphicsPipelineInstance.get());
#endif // _DEBUG
	GameWorldInstance->LoadGameWorld();
}

PortfolioApp::~PortfolioApp()
{
	DestroyWindow(MainWindow);
	UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
}


void PortfolioApp::LoadMonitorInfo()
{
	HMONITOR Monitor = MonitorFromWindow(MainWindow, MONITOR_DEFAULTTONEAREST);
	MONITORINFO MonitorInfo;
	AutoZeroMemory(MonitorInfo);
	MonitorInfo.cbSize = sizeof(MONITORINFO);

	bool MonitorResult = GetMonitorInfoA(Monitor, &MonitorInfo);
	assert(MonitorResult);
	
	DWORD Style = GetWindowLong(MainWindow, GWL_STYLE);
	DWORD ExStyle = GetWindowLong(MainWindow, GWL_EXSTYLE);

	App::GWidth = MonitorInfo.rcWork.right - MonitorInfo.rcWork.left;
	App::GHeight = MonitorInfo.rcWork.bottom - MonitorInfo.rcWork.top;
}

void PortfolioApp::OnChangeWindow()
{
	RECT Rect;
	if (GetClientRect(MainWindow, &Rect))
	{
		App::GWidth = Rect.right - Rect.left;
		App::GHeight = Rect.bottom - Rect.top;

		GraphicsPipelineInstance->ResizeSwapChain(App::GWidth, App::GHeight);
	}
}

void PortfolioApp::Run()
{
	// Main message loop
	MSG msg = {};
	while (WM_QUIT != msg.message) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			Update();
			Render();
		}
	}
}

void PortfolioApp::Quit()
{
}

void PortfolioApp::Update()
{
	const float& DeltaTime = GetDeltaTimeFromLastCall();
	GameWorldInstance->UpdateGameWorld(DeltaTime);
}

void PortfolioApp::Render()
{
	GameWorldInstance->RenderWorld();
	GraphicsPipelineInstance->GetSwapChain()->Present(0, 0);
}

float PortfolioApp::GetDeltaTimeFromLastCall()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	LONGLONG Elapsed = CurrentTime.QuadPart - PrevTime.QuadPart;
	const float DeltaTime = static_cast<float>(Elapsed) / Frequency.QuadPart;

	PrevTime = CurrentTime;
	return DeltaTime;
}

LRESULT __stdcall PortfolioApp::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ManageMessage(hWnd, msg, wParam, lParam);

	if (GameWorldInstance)
		GameWorldInstance->AppProc(hWnd, msg, wParam, lParam);

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void PortfolioApp::ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_EXITSIZEMOVE:
		OnChangeWindow();
		break;
	case WM_CREATE:
		DragAcceptFiles(hWnd, TRUE);
		break;
	}
}
