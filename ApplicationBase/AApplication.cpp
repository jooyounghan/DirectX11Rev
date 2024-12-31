#include "AApplication.h"
#include <assert.h>
#include <functional>

using namespace std;
using namespace App;

App::AApplication* App::AApplication::MainApp = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return App::AApplication::MainApp->AppProc(hWnd, msg, wParam, lParam);
}

AApplication::AApplication()
{
	SetDefaultHandler();
}


SWindowSize AApplication::LoadMonitorInfo()
{
	HMONITOR monitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo;

	ZeroMemory(&monitorInfo, sizeof(MONITORINFO));

	monitorInfo.cbSize = sizeof(MONITORINFO);

	bool monitorResult = GetMonitorInfoA(monitor, &monitorInfo);
	assert(monitorResult);

	SWindowSize result;
	result.m_width = monitorInfo.rcWork.right - monitorInfo.rcWork.left;
	result.m_height = monitorInfo.rcWork.bottom - monitorInfo.rcWork.top;
	return result;
}

void App::AApplication::Init(const wchar_t* className, const wchar_t* applicaitonName)
{
	m_appSize = LoadMonitorInfo();
	ZeroMemory(&m_windowClass, sizeof(m_windowClass));
	m_windowClass = {
		sizeof(WNDCLASSEX), CS_CLASSDC, WndProc,
		0L, 0L, GetModuleHandle(NULL),
		NULL, NULL, NULL,
		NULL, className, NULL
	};
	RegisterClassEx(&m_windowClass);

	HWND hWindow = CreateWindow(
		m_windowClass.lpszClassName, applicaitonName,
		WS_OVERLAPPEDWINDOW, 0, 0,
		m_appSize.m_width, m_appSize.m_height, NULL,
		NULL, m_windowClass.hInstance, NULL
	);
	m_mainWindow = hWindow;

	ShowWindow(m_mainWindow, SW_SHOW);

	InitTimer();
}

void AApplication::Run()
{
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
			const float& deltaTime = GetDeltaTimeFromLastCall();
			Update(deltaTime);
		}
	}
}

void AApplication::Quit()
{
}

LRESULT __stdcall AApplication::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_EXITSIZEMOVE:
		OnWindowSizeMove();
		break;
	case WM_CREATE:
		DragAcceptFiles(hWnd, TRUE);
		break;
	}
	AppProcImpl(hWnd, msg, wParam, lParam);
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void AApplication::OnWindowSizeMove()
{
	RECT Rect;
	if (GetClientRect(m_mainWindow, &Rect))
	{
		m_appSize.m_width = Rect.right - Rect.left;
		m_appSize.m_height = Rect.bottom - Rect.top;
		m_onWindowSizeMoveHandler(m_appSize.m_width, m_appSize.m_height);
	}
}

void AApplication::SetDefaultHandler()
{
	App::AApplication::MainApp = this;
	m_onWindowSizeMoveHandler = [&](const UINT&, const UINT&) {};
}

void AApplication::InitTimer()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_prevTime);
}

float AApplication::GetDeltaTimeFromLastCall()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	LONGLONG Elapsed = currentTime.QuadPart - m_prevTime.QuadPart;
	const float DeltaTime = static_cast<float>(Elapsed) / m_frequency.QuadPart;

	m_prevTime = currentTime;
	return DeltaTime;
}

