#include "GameWorld.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "PSOManager.h"

#ifdef _DEBUG
#include "EditorWorld.h"
#endif 

#include "Camera.h"
#include "Map.h"

using namespace std;

#ifdef _DEBUG
GameWorld::GameWorld(GraphicsPipeline* GraphicsPipelineInstance, HWND WindowHandle)
	: IWorld(GraphicsPipelineInstance)
{
	PSOManagerInstance = make_unique<PSOManager>((GraphicsPipelineInstance));

	EditorWorldInstance = make_unique<EditorWorld>(this, WindowHandle);
	CurrentCamera = EditorWorldInstance->GetEditorCamera();
	CurrentCamera->SetPosition(0.f, 0.f, -300.f);

	MapInstances.emplace(0, std::move(make_unique<Map>(GraphicsPipelineInstance, PSOManagerInstance.get())));
	CurrentMap = MapInstances[0].get();
}
#else
GameWorld::GameWorld(GraphicsPipeline* GraphicsPipelineInstance)
	: IWorld(GraphicsPipelineInstance)
{
	PSOManagerInstance = make_unique<PSOManager>((GraphicsPipelineInstance));

	TestCamera = make_unique<Camera>(GraphicsPipelineInstance, App::GWidth, App::GHeight);
	CurrentCamera = TestCamera.get();
	CurrentCamera->SetPosition(0.f, 0.f, -300.f);

	MapInstances.emplace(0, std::move(make_unique<Map>(GraphicsPipelineInstance, PSOManagerInstance.get())));
	CurrentMap = MapInstances[0].get();
}
#endif // _DEBUG


GameWorld::~GameWorld()
{

}

void GameWorld::LoadGameWorld()
{
	// Load Current State Of GameWorld From Binary Files
}

void GameWorld::UpdateGameWorld(const float& DeltaTimeIn)
{
	if (CurrentMap)
	{
		CurrentMap->UpdateMap(DeltaTimeIn);
		if (CurrentCamera)
		{
			CurrentCamera->UpdateView();
		}
	}
}

void GameWorld::RenderWorld()
{
	if (CurrentMap)
	{
		if (CurrentCamera)
		{
			CurrentMap->RenderMap(CurrentCamera);
			
#ifdef _DEBUG
			EditorWorldInstance->RenderWorld();
#else
			GraphicsPipelineCached->GetDeviceContext()->CopyResource(
				GraphicsPipelineCached->GetBackBufferTexture(),
				CurrentCamera->GetSceneTexture2D()
			);
#endif // _DEBUG
		}
	}

}

LRESULT GameWorld::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	return EditorWorldInstance->AppProc(hWnd, msg, wParam, lParam);
#else
	switch (msg)
	{
	case WM_SIZE:
		return 0;
	case WM_EXITSIZEMOVE:
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
#endif
}
