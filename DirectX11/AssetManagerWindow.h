#pragma once
#include "IWindow.h"

class AssetManager;

class AssetManagerWindow : public IWindow
{
public:
	AssetManagerWindow(AssetManager* AssetManagerIn);
	virtual ~AssetManagerWindow();

private:
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void RenderWindow() override;

private:
	void RenderAssetFileStructure();

private:
	void RenderCurrentDirectoryAsset();
};

