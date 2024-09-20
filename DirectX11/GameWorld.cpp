#include "GameWorld.h"

#include "AssetManager.h"

#include "EditorWorld.h"
#include "MapAsset.h"

using namespace std;


GameWorld::GameWorld(HWND WindowHandle)
{
	AssetManagerInstance = make_unique<AssetManager>();

#ifdef _DEBUG
	EditorWorldInstance = make_unique<EditorWorld>(this, WindowHandle);
#else

#endif
	LoadManagingMaps();
}


GameWorld::~GameWorld()
{
}

void GameWorld::LoadManagingMaps()
{
	static UINT MapCount = 0;

	const unordered_map<string, shared_ptr<MapAsset>>& ManagingMaps = AssetManagerInstance->GetManagingMaps();

	if (ManagingMaps.empty())
	{
		MapInstances.emplace(MapCount, make_shared<MapAsset>("TestMap", AssetManagerInstance.get(), false));
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
}

void GameWorld::Update(const float& DeltaTimeIn)
{
	if (CurrentMap)
	{
		CurrentMap->Update(DeltaTimeIn);
	}
}

void GameWorld::RenderWorld()
{
	
	if (CurrentMap)
	{
		CurrentMap->RenderMap();
	}

	EditorWorldInstance->RenderWorld();
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