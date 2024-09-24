#include "PortfolioApp.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"

#include "GraphicsPipeline.h"
#include "PSOManager.h"
#include "InputEventManager.h"
#include "GameWorld.h"

using namespace std;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return App::GApp->AppProc(hWnd, msg, wParam, lParam);
}

PortfolioApp::PortfolioApp()
{
	App::GApp = this;

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

	ResetCursorHotSpot(0, 0);

	GraphicsPipelineInstance = make_unique<GraphicsPipeline>();
	GraphicsPipelineInstance->LoadPipeline(App::GWidth, App::GHeight, hWindow);
	App::GGraphicPipeline = GraphicsPipelineInstance.get();

	PSOManagerInstance = make_unique<PSOManager>();
	App::GPSOManager = PSOManagerInstance.get();

	OnChangeWindow();

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&PrevTime);

	InputEventManagerInstance = make_unique<InputEventManager>();
	App::GInputEventManager = InputEventManagerInstance.get();

#ifdef _DEBUG
	GameWorldInstance = make_unique<GameWorld>(MainWindow);
#else
	GameWorldInstance = make_unique<GameWorld>();
#endif // _DEBUG
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
			if (GameWorldInstance != nullptr && GraphicsPipelineInstance != nullptr)
			{
				Update();
				Render();
			}
		}
	}
}

void PortfolioApp::Quit()
{
}

void PortfolioApp::Update()
{
	const float& DeltaTime = GetDeltaTimeFromLastCall();

	InputEventManagerInstance->Update(DeltaTime);
	GameWorldInstance->Update(DeltaTime);
}

void PortfolioApp::Render()
{
	GameWorldInstance->Render();
	GraphicsPipelineInstance->GetSwapChain()->Present(0, 0);
}

void PortfolioApp::ResetCursorHotSpot(const DWORD& HotspotX, const DWORD& HotspotY)
{
	HCURSOR CurrentCursor = GetCursor();
	if (!CurrentCursor)  { return; }

	ICONINFO IconInfo;
	if (!GetIconInfo(CurrentCursor, &IconInfo)) { return; }

	IconInfo.xHotspot = HotspotX;
	IconInfo.yHotspot = HotspotY;

	HCURSOR NewCursor = CreateIconIndirect(&IconInfo);

	if (IconInfo.hbmMask) 
	{
		DeleteObject(IconInfo.hbmMask);
	}
	if (IconInfo.hbmColor)
	{
		DeleteObject(IconInfo.hbmColor);
	}

	if (NewCursor) 
	{
		SetCursor(NewCursor);
	}
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
	
	InputEventManagerInstance->ManageInput(msg, wParam, lParam);

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
	case WM_DESTROY:
		GraphicsPipelineInstance.reset();
		GameWorldInstance.reset();
		break;
	}
}
