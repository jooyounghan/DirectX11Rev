#pragma once
#include "ANodeCanvas.h"

class AssetManager;
class AAssetFile;

class AAssetNodeCanvas : public ANodeCanvas
{
public:
	AAssetNodeCanvas(AssetManager* AssetManagerIn, AAssetFile* AssetFileIn, const ImVec2& CanvasSizeIn);
	virtual ~AAssetNodeCanvas();

protected:
	AssetManager* AssetManagerCached = nullptr;
	AAssetFile* AssetFileCached = nullptr;
};

