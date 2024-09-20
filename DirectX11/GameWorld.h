#pragma once
#include "IWorld.h"
#include "HeaderHelper.h"

#include <d3d11.h>
#include <memory>
#include <unordered_map>

class PSOManager;
class AssetManager;
class EditorWorld;
class AAssetFile;
class MapAsset;
class Camera;

class GameWorld : public IWorld
{
public:
#ifdef _DEBUG
	GameWorld(HWND WindowHandle);
#else
	GameWorld();
	std::unique_ptr<Camera> TestCamera;
#endif // _DEBUG
	virtual ~GameWorld();

public:
	void LoadManagingMaps();
	virtual void Update(const float& DeltaTimeIn) override;

protected:
	std::unique_ptr<PSOManager> PSOManagerInstance;
	MakeSmartPtrGetter(PSOManagerInstance);

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
	MapAsset* CurrentMap = nullptr;
	MakeGetter(CurrentMap);
	
protected:
	Camera* CurrentCamera = nullptr;

public:
	virtual void RenderWorld() override;

private:
	void RenderBeforeToneMapping();
	void ToneMapping();
	void RenderAfterToneMapping();

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};

