#pragma once
#include "IWorld.h"
#include "HeaderHelper.h"

#include <d3d11.h>
#include <memory>
#include <unordered_map>

class PSOManager;
class EditorWorld;

class Map;
class Camera;

class GameWorld : public IWorld
{
public:
#ifdef _DEBUG
	GameWorld(GraphicsPipeline* GraphicsPipelineInstance, HWND WindowHandle);
#else
	GameWorld(GraphicsPipeline* GraphicsPipelineInstance);
	std::unique_ptr<Camera> TestCamera;
#endif // _DEBUG
	~GameWorld();

public:
	void LoadGameWorld();
	void UpdateGameWorld(const float& DeltaTimeIn);

protected:
	std::unique_ptr<PSOManager> PSOManagerInstance;
	MakeSmartPtrGetter(PSOManagerInstance);

#ifdef _DEBUG
protected:
	std::unique_ptr<EditorWorld> EditorWorldInstance;
	MakeSmartPtrGetter(EditorWorldInstance)
#endif // _DEBUG

protected:
	std::unordered_map<UINT, std::unique_ptr<Map>> MapInstances;
	Map* CurrentMap = nullptr;
	MakeGetter(CurrentMap);

public:
	virtual void RenderWorld() override;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

