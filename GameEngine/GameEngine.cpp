#include "GameEngine.h"

GameEngine::GameEngine()
	: AApplication(), g_engineDoublePtr(D3D11::D3D11Engine::GetInstance())
{

}

void GameEngine::Init(const wchar_t* className, const wchar_t* applicaitonName)
{
	AApplication::Init(className, applicaitonName);
	D3D11::D3D11Engine* engine = GetEngine();
	if (engine != nullptr)
	{
		engine->InitEngine(m_appSize.width, m_appSize.height, m_mainWindow);
	}
}

void GameEngine::Update(const float& deltaTime)
{

}

void GameEngine::AppProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


}
