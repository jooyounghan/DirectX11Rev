#pragma once
#include "VariableInputNode.h"

class BaseTextureAsset;

class BaseTextureInputNode : public VariableInputNode
{
public:
	BaseTextureInputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		std::shared_ptr<BaseTextureAsset> BaseTextureAssetIn
	);
	virtual ~BaseTextureInputNode();

protected:
	std::shared_ptr<BaseTextureAsset> BaseTextureAssetCached;
};

