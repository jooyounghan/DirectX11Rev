#pragma once
#include "AAssetNodeCanvas.h"

class MaterialAsset;

class MaterialAssetNodeCanvas : public AAssetNodeCanvas
{
public:
	MaterialAssetNodeCanvas(AssetManager* AssetMangerIn, AAssetFile* AssetFileIn);
	virtual ~MaterialAssetNodeCanvas();

protected:
	MaterialAsset* MaterialAssetCached = nullptr;

protected:
	virtual void ShowContextMenu() override;
};

