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
	PSOManagerInstance = make_unique<PSOManager>((GraphicsPipelineInstance->Device.Get()));

	TestCamera = make_unique<Camera>(GraphicsPipelineInstance, Graphics::GWidth, Graphics::GHeight);
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
			CurrentMap->CaptureMapByCamera(CurrentCamera);
		}
	}
}

void GameWorld::RenderWorld()
{
#ifdef _DEBUG
	EditorWorldInstance->RenderWorld();
#else
	if (CurrentCamera != nullptr)
	{
		ID3D12GraphicsCommandList* RenderCommandList = GraphicsPipelineCached->CommandLists[ECommandType::Rendering].Get();
		RenderCommandList->CopyResource(
			GraphicsPipelineCached->GetCurrentBackBuffer()->GetResource(),
			CurrentCamera->Texture2D->GetResource()
		);
	}
#endif // _DEBUG
}