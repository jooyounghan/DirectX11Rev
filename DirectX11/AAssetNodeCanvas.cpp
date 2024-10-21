#include "AAssetNodeCanvas.h"

AAssetNodeCanvas::AAssetNodeCanvas(
	AssetManager* AssetManagerIn, 
	AAssetFile* AssetFileIn, 
	const ImVec2& CanvasSizeIn
)
	: ANodeCanvas(CanvasSizeIn), AssetManagerCached(AssetManagerIn), AssetFileCached(AssetFileIn)
{
}

AAssetNodeCanvas::~AAssetNodeCanvas()
{
}

void AAssetNodeCanvas::RenderControl()
{
	ANodeCanvas::RenderControl();
}
