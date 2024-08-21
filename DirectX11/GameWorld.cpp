#include "GameWorld.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "PSOManager.h"
#include "AssetManager.h"

#ifdef _DEBUG
#include "EditorWorld.h"
#include "EditorActor.h"
#include "EditorCamera.h"
#endif 

#include "Camera.h"
#include "MapAsset.h"

using namespace std;

#ifdef _DEBUG
GameWorld::GameWorld(GraphicsPipeline* GraphicsPipelineInstance, HWND WindowHandle)
	: IWorld(GraphicsPipelineInstance)
{
	PSOManagerInstance = make_unique<PSOManager>((GraphicsPipelineInstance));
	AssetManagerInstance = make_unique<AssetManager>(GraphicsPipelineCached);
	EditorWorldInstance = make_unique<EditorWorld>(this, WindowHandle);

	auto tests = AssetManagerInstance->GetManagingMaps();

	if (tests.empty())
	{
		MapInstances.emplace(0, make_shared<MapAsset>("TestMap", AssetManagerInstance.get(), true));
		CurrentMap = MapInstances[0].get();
	}
	else
	{
		UINT idx = 0;
		for (auto& test : tests)
		{
			MapInstances.emplace(idx, test.second);
		}
		CurrentMap = MapInstances[0].get();
	}

	EditorActor* EditorActorInstnace = EditorWorldInstance->GetEditorActorInstance();
	if (EditorActorInstnace != nullptr)
	{
		CurrentMap->SetCameraCached(EditorActorInstnace->GetEditorCameraCached());
	}
}
#else
GameWorld::GameWorld(GraphicsPipeline* GraphicsPipelineInstance)
	: IWorld(GraphicsPipelineInstance)
{
	PSOManagerInstance = make_unique<PSOManager>((GraphicsPipelineInstance));
	AssetManagerInstance = make_unique<AssetManager>(GraphicsPipelineCached->GetDevice());

	TestCamera = make_unique<Camera>(GraphicsPipelineInstance, App::GWidth, App::GHeight);
	TestCamera->Position.z = -300.f;

	MapInstances.emplace(0, std::move(make_unique<Map>(GraphicsPipelineInstance, PSOManagerInstance.get(), AssetManagerInstance.get())));
	CurrentMap = MapInstances[0].get();
}
#endif // _DEBUG


GameWorld::~GameWorld()
{
	bool test = true;
}

void GameWorld::LoadGameWorld()
{
	// Load Current State Of GameWorld From Binary Files
}

void GameWorld::UpdateWorld(const float& DeltaTimeIn)
{
#ifdef _DEBUG
	EditorWorldInstance->UpdateWorld(DeltaTimeIn);
#endif // _DEBUG

	if (CurrentMap)
	{
		CurrentMap->UpdateMap(DeltaTimeIn);
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