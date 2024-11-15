#pragma once
class MapAsset;

class IRenderable
{
public:
	virtual void Render(MapAsset* MapAssetIn) = 0;
};