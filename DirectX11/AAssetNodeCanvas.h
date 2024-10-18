#pragma once
#include "ANodeCanvas.h"

class AssetManager;
class AAssetFile;

class AAssetNodeCanvas : public ANodeCanvas
{
public:
	AAssetNodeCanvas(AssetManager* AssetManagerIn, AAssetFile* AssetFileIn);
	virtual ~AAssetNodeCanvas();

protected:
	AssetManager* AssetManagerCached = nullptr;
	AAssetFile* AssetFileCached = nullptr;

protected:
	virtual void ShowContextMenu() = 0;
};

