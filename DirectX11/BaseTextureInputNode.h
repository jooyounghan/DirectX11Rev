#pragma once
#include "VariableInputNode.h"

class AssetManager;
class BaseTextureAsset;
class AssetVariableInputPort;
class BaseTextureOutputPort;

class BaseTextureInputNode : public VariableInputNode
{
public:
	BaseTextureInputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		AssetManager* AssetManagerIn,
		std::shared_ptr<BaseTextureAsset> BaseTextureAssetIn
	);
	virtual ~BaseTextureInputNode();

protected:
	AssetManager* AssetManagerCached = nullptr;
	std::shared_ptr<BaseTextureAsset> BaseTextureAssetCached;
	
protected:
	BaseTextureOutputPort* BaseTextureOutputPortCached = nullptr;
	MakeGetter(BaseTextureOutputPortCached);

protected:
	const float ImageSizeX;
	const float ImageSizeY;
	const ImVec2 ImageSize;

protected:
	const float ComboWidth;
	const ImVec2 ImagePosition;
	const ImVec2 TextBoxPosition;

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
};

