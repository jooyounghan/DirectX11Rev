#pragma once
#include "AApplication.h"

#include "D3D11Engine.h"
#include "AWindow.h"
#include "AModal.h"

#include <memory>

class DefferedContext;
class TaskManager;
class SessionManager;
class AssetManager;
class ComponentManager;
class ComponentPSOManager;
class CameraComponent;
class InputEventManager;

class GameEngine : public App::AApplication
{
public:
	GameEngine();
	~GameEngine() override;


protected:
	D3D11::D3D11Engine* m_engine;

protected:
	std::vector<AWindow*> m_imguiWindows;
	std::vector<AModal*> m_imguiModals;

protected:
	TaskManager* m_taskManager = nullptr;
	SessionManager* m_sessionManager = nullptr;
	AssetManager* m_assetManager = nullptr;
	ComponentManager* m_componentManager = nullptr;
	ComponentPSOManager* m_componentPSOManager = nullptr;

protected:
	CameraComponent* m_editorCamera = nullptr;

protected:
	static Utilities::SColor ClearColor;

public:
	virtual void Init(const wchar_t* className, const wchar_t* applicaitonName) override;

private:
	virtual void Update(const float& deltaTime) override;
	virtual void AppProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void OnDropFiles(const HDROP& hDrop);

private:
	void CreateEditor();

private:
	void CreateDefferedContext();
	void CreateSessionManager();
	void CreateAssetManager();
	void CreateComponentManager();
	void CreatePSOManager();
	void CreateWindows();
	void CreateModals();
};

