#pragma once
#include "IWindow.h"

class EditorWorld;
class EditorPawn;
class IDSelectCamera; 

class GameWorld;
class AssetManager;
class MapAsset;

class ViewportWindow : public IWindow
{
public: 
	ViewportWindow(EditorWorld* EditorWorldIn);
	virtual ~ViewportWindow();

public:
	virtual void RenderWindow() override;

private:
	EditorWorld* EditorWorldCached = nullptr;
	GameWorld* GameWorldCached = nullptr;
	AssetManager* AssetManagerCached = nullptr;
	MapAsset* CurrentMap = nullptr;

private:
	EditorPawn* EditorActorCached = nullptr;
	IDSelectCamera* IDSelectCameraCached = nullptr;

private:
	ImVec2 ImagePosition = ImVec2();
	ImVec2 ImageSize = ImVec2();

private:
	void ManageAssetDrop();
	void ManageMouseLBClick();
};

