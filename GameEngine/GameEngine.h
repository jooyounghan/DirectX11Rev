#pragma once
#include "AApplication.h"
#include "D3D11Engine.h"
#include "AssetManager.h"
#include "DefferedContext.h"
#include "TaskManager.h"
#include "AWindow.h"
#include "AModal.h"

#include <memory>

enum class EDefferedContextType
{
	ASSET,
	INIT_WINDOW
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
		D3D11::D3D11Engine** m_engineDoublePtr;
		std::unordered_map<EDefferedContextType, DefferedContext*> m_defferedContexts;
		
	protected:
		std::vector<AWindow*> m_imguiWindows;
		std::vector<AModal*> m_imguiModals;

	protected:
		AssetManager* m_assetManager;

	protected:
		TaskManager m_taskManager;
	
	protected:
		static Utilities::SColor ClearColor;

	public:
		inline D3D11::D3D11Engine* GetEngine() { return *m_engineDoublePtr; }
	};
}

