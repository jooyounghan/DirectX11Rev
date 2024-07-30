#include "GameWorld.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "PSOManager.h"
#include "AssetManager.h"

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
	AssetManagerInstance = make_unique<AssetManager>(GraphicsPipelineCached->GetDevice());
	EditorWorldInstance = make_unique<EditorWorld>(this, WindowHandle);

	MapInstances.emplace(0, std::move(make_unique<Map>(GraphicsPipelineInstance, PSOManagerInstance.get(), AssetManagerInstance.get())));
	CurrentMap = MapInstances[0].get();
}
#else
GameWorld::GameWorld(GraphicsPipeline* GraphicsPipelineInstance)
	: IWorld(GraphicsPipelineInstance)
{
	PSOManagerInstance = make_unique<PSOManager>((GraphicsPipelineInstance));
	AssetManagerInstance = make_unique<AssetManager>(GraphicsPipelineCached->GetDevice());

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
	}
}

void GameWorld::RenderWorld()
{
	if (CurrentMap)
	{
		CurrentMap->RenderMap();
			
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

void GameWorld::AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	EditorWorldInstance->AppProc(hWnd, msg, wParam, lParam);
#endif

	ManageMessage(hWnd, msg, wParam, lParam);
}

void GameWorld::ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	default:
		break;
	}
}