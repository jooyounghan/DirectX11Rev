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
		NULL, NULL, NULL, 
		NULL, WindowClass.hInstance, NULL
	);

	MainWindow = hWindow;

	LoadMonitorInfo();

	GraphicsPipelineInstance = make_unique<GraphicsPipeline>();
	GraphicsPipelineInstance->LoadPipeline(App::GWidth, App::GHeight, hWindow);

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

	App::GWidth = MonitorInfo.rcWork.right - MonitorInfo.rcWork.left;
	App::GHeight = MonitorInfo.rcWork.bottom - MonitorInfo.rcWork.top;
	SetWindowPos(MainWindow, NULL, 0, 0, App::GWidth, App::GHeight, SWP_SHOWWINDOW | SWP_NOMOVE);
	UpdateWindow(MainWindow);
}

void PortfolioApp::ChangeMainFrameSize(const UINT& WidthIn, const UINT& HeightIn)
{
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
	if (GameWorldInstance)
		return GameWorldInstance->AppProc(hWnd, msg, wParam, lParam);
	else
		return ::DefWindowProc(hWnd, msg, wParam, lParam);

}
