#pragma once

#include <windows.h>
#include <windowsx.h>
#include <wrl/client.h>
#include <memory>

class GraphicsPipeline;
class PSOManager;
class InputEventManager;
class UploadableBufferManager;
class GameWorld;

class PortfolioApp
{
public:
	PortfolioApp();
	~PortfolioApp();

public:
	static PortfolioApp* GApp;

protected:
	WNDCLASSEX WindowClass;
	HWND MainWindow;

protected:
	LARGE_INTEGER Frequency = LARGE_INTEGER();
	LARGE_INTEGER PrevTime = LARGE_INTEGER();

protected:
	std::unique_ptr<GraphicsPipeline>			GraphicsPipelineInstance;
	std::unique_ptr<PSOManager>					PSOManagerInstance;
	std::unique_ptr<InputEventManager>			InputEventManagerInstance;
	std::unique_ptr<UploadableBufferManager>	UploadableBufferManagerInstance;
	std::unique_ptr<GameWorld>					GameWorldInstance;

private:
	void LoadMonitorInfo();

private:
	void OnChangeWindow();

public:
	void Run();
	void Quit();

protected:
	void Update();
	void Render();

private:
	void ResetCursorHotSpot(const DWORD& HotspotX, const DWORD& HotspotY);

private:
	float GetDeltaTimeFromLastCall();

public:
	LRESULT WINAPI AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

