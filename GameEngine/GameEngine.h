#pragma once
#include "AApplication.h"
#include "D3D11Engine.h"
#include "AImGuiWindow.h"
#include <memory>

class GameEngine : public App::AApplication
{
public:
	GameEngine();

public:
	virtual void Init(const wchar_t* className, const wchar_t* applicaitonName) override;

protected:	
	virtual void Update(const float& deltaTime) override;
	virtual void AppProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

protected:
	D3D11::D3D11Engine** m_engineDoublePtr;
	std::vector<AImGuiWindow*> m_imguiWindows;

protected:
	static Utilities::SColor ClearColor;

public:
	inline D3D11::D3D11Engine* GetEngine() { return *m_engineDoublePtr; }
};