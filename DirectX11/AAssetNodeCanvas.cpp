#include "AAssetNodeCanvas.h"

AAssetNodeCanvas::AAssetNodeCanvas(
	AAssetFile* AssetFileIn, 
	const ImVec2& CanvasSizeIn
)
	: ANodeCanvas(CanvasSizeIn), AssetFileCached(AssetFileIn)
{
}

AAssetNodeCanvas::~AAssetNodeCanvas()
{
}

void AAssetNodeCanvas::RenderControl()
{
	ANodeCanvas::RenderControl();
}
