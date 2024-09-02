#include "GameWorld.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "PSOManager.h"
#include "AssetManager.h"

#ifdef _DEBUG
#include "EditorWorld.h"
#include "EditorPawn.h"
#include "EditorCamera.h"
#endif 

#include "Camera.h"
#include "MapAsset.h"

using namespace std;

#ifdef _DEBUG
GameWorld::GameWorld(HWND WindowHandle)
{
	PSOManagerInstance = make_unique<PSOManager>();
	AssetManagerInstance = make_unique<AssetManager>();
	EditorWorldInstance = make_unique<EditorWorld>(this, WindowHandle);

	LoadManagingMaps();
}
#else
GameWorld::GameWorld()
	: IWorld()
{
	PSOManagerInstance = make_unique<PSOManager>();
	AssetManagerInstance = make_unique<AssetManager>();

	TestCamera = make_unique<Camera>(App::GWidth, App::GHeight);
	TestCamera->Position.z = -300.f;

	LoadManagingMaps();
}
#endif // _DEBUG


GameWorld::~GameWorld()
{
}

void GameWorld::LoadManagingMaps()
{
	static UINT MapCount = 0;

	const unordered_map<string, shared_ptr<MapAsset>>& ManagingMaps = AssetManagerInstance->GetManagingMaps();

	if (ManagingMaps.empty())
	{
		MapInstances.emplace(MapCount, make_shared<MapAsset>("TestMap", AssetManagerInstance.get(), true));
		CurrentMap = (MapAsset*)MapInstances[MapCount].get();
		MapCount++;
	}
	else
	{
		for (auto& ManagingMap : ManagingMaps)
		{
			MapInstances.emplace(MapCount, ManagingMap.second);
			MapCount++;
		}
	}

	// TODO : Map을 선택하는 과정 추가하기
	CurrentMap = MapInstances[0].get();

	EditorPawn* EditorActorInstnace = EditorWorldInstance->GetEditorActorInstance();
	if (EditorActorInstnace != nullptr)
	{
		CurrentMap->SetCameraCached(EditorActorInstnace->GetEditorCameraCached());
	}
}

void GameWorld::Update(const float& DeltaTimeIn)
{
#ifdef _DEBUG
	EditorWorldInstance->Update(DeltaTimeIn);
#endif // _DEBUG

	if (CurrentMap)
	{
		CurrentMap->Update(DeltaTimeIn);
	}
}

void GameWorld::RenderWorld()
{
	if (CurrentMap)
	{
		CurrentMap->RenderMap(PSOManagerInstance.get());
			
#ifdef _DEBUG
		EditorWorldInstance->RenderWorld();
#else
		GraphicsPipelineCached->GetDeviceContext()->CopyResource(
			GraphicsPipelineCached->GetBackBufferTexture(),
			TestCamera->GetSceneTexture2D()
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
	case NULL:
		break;
	default:
		break;
	}
}