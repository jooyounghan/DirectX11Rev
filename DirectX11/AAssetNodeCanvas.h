#pragma once
#include "ANodeCanvas.h"

class AAssetFile;

class AAssetNodeCanvas : public ANodeCanvas
{
public:
	AAssetNodeCanvas(AAssetFile* AssetFileIn, const ImVec2& CanvasSizeIn);
	virtual ~AAssetNodeCanvas();

protected:
	AAssetFile* AssetFileCached = nullptr;

public:
	virtual void RenderControl() override;
};

