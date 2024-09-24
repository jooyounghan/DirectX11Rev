#pragma once
#include "IWorld.h"
#include "IUpdatable.h"
#include "HeaderHelper.h"

#include <d3d11.h>
#include <memory>
#include <unordered_map>

class AssetManager;
class EditorWorld;
class MapAsset;

class GameWorld : public IWorld, public IUpdatable
{
public:
#ifdef _DEBUG	
	GameWorld(HWND WindowHandle);
#else
	GameWorld();
#endif // _DEBUG
	virtual ~GameWorld();

public:
	void LoadManagingMaps();
	virtual void Update(const float& DeltaTimeIn) override;

protected:
	std::unique_ptr<AssetManager> AssetManagerInstance;
	MakeSmartPtrGetter(AssetManagerInstance);

#ifdef _DEBUG
protected:
	std::unique_ptr<EditorWorld> EditorWorldInstance;
	MakeSmartPtrGetter(EditorWorldInstance)
#endif // _DEBUG

protected:
	std::unordered_map<UINT, std::shared_ptr<MapAsset>> MapInstances;
	MakeGetter(MapInstances);

protected:
	MapAsset* CurrentMap = nullptr;
	MakeGetter(CurrentMap);

public:
	virtual void Render() override;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

