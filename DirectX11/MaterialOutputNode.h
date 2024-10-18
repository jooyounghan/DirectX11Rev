#pragma once
#include "VariableOutputNode.h"

class MaterialAsset;

class MaterialOutputNode : public VariableOutputNode
{
public:
	MaterialOutputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		std::shared_ptr<MaterialAsset> MaterialAssetIn
	);
	virtual ~MaterialOutputNode();

protected:
	std::shared_ptr<MaterialAsset> MaterialAssetCached;
};

