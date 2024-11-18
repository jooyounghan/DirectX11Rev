#pragma once
class MapAsset;

class IRenderable
{
public:
	virtual void Render(MapAsset* MapAssetIn) = 0;
	//virtual void RenderDeffered(MapAsset* MapAssetIn) = 0;
};