#pragma once
#include <windows.h>
#include <functional>

namespace App
{
	struct SWindowSize
	{
		UINT width;
		UINT height;
	};

	using WindowSizeMoveHandler = std::function<void(const UINT&, const UINT&)>;


	class AApplication
	{
	public:
		AApplication();
		virtual ~AApplication();

	public:
		static AApplication* MainApp;

	protected:
		WNDCLASSEX m_windowClass;
		HWND m_mainWindow;


	protected:
		SWindowSize m_appSize;
		LARGE_INTEGER m_frequency;
		LARGE_INTEGER m_prevTime;

	protected:
		WindowSizeMoveHandler	m_onWindowSizeMoveHandler;

	protected:
		static SWindowSize LoadMonitorInfo();

	public:
		virtual void Init(const wchar_t* className, const wchar_t* applicaitonName);
		void Run();
		void Quit();

	public:
		LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		void OnWindowSizeMove();
		void OnDropFiles();

	protected:
		virtual void Update(const float& deltaTime) = 0;
		virtual void AppProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	private:
		void SetDefaultHandler();
		void InitTimer();
		float GetDeltaTimeFromLastCall();
	};


}
