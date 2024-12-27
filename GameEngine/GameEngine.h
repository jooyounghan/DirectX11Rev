#pragma once
#include "AApplication.h"

#include "D3D11Engine.h"
#include "DefferedContext.h"

#include "AssetManager.h"
#include "TaskManager.h"

#include "SessionManager.h"
#include "ComponentManager.h"

#include "AWindow.h"
#include "AModal.h"

#include <memory>

enum class EDefferedContextType
{
	ASSETS,
	WINDOWS
};

namespace YHEngine
{
	class GameEngine : public App::AApplication
	{
	public:
		GameEngine();
		virtual ~GameEngine();

	public:
		virtual void Init(const wchar_t* className, const wchar_t* applicaitonName) override;

	protected:
		virtual void Update(const float& deltaTime) override;
		virtual void AppProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	protected:
		void OnDropFiles(const HDROP& hDrop);

	protected:
		D3D11::D3D11Engine* m_engine;
		TaskManager* m_taskManager = nullptr;

	protected:
		SessionManager m_sessionManager;

	protected:
		std::unordered_map<EDefferedContextType, DefferedContext*> m_defferedContexts;
		
	protected:
		std::vector<AWindow*> m_imguiWindows;
		std::vector<AModal*> m_imguiModals;

	protected:
		AssetManager* m_assetManager = nullptr;

	protected:
		ComponentManager* m_componentManager = nullptr;

	protected:
		static Utilities::SColor ClearColor;
	};
}

