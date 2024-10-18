#include "AAssetNodeCanvas.h"

AAssetNodeCanvas::AAssetNodeCanvas(
	AssetManager* AssetManagerIn, 
	AAssetFile* AssetFileIn
)
	: AssetManagerCached(AssetManagerIn), AssetFileCached(AssetFileIn)
{
}

AAssetNodeCanvas::~AAssetNodeCanvas()
{
}
