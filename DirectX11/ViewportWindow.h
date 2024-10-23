#pragma once
#include "AWindow.h"

class EditorWorld;
class EditorPawn;
class ACamera; 

class GameWorld;
class AssetManager;
class MapAsset;

class ViewportWindow : public AWindow
{
public: 
	ViewportWindow(EditorWorld* EditorWorldCached);
	virtual ~ViewportWindow();

public:
	virtual void RenderWindow() override;

private:
	EditorWorld* EditorWorldCached = nullptr;
	GameWorld* GameWorldCached = nullptr;
	AssetManager* AssetManagerCached = nullptr;
	MapAsset* CurrentMap = nullptr;

private:
	ACamera* CameraCached = nullptr;

private:
	ImVec2 ImagePosition = ImVec2();
	ImVec2 ImageSize = ImVec2();

private:
	void ManageAssetDrop();
	void ManageMouseLBClick();
};

