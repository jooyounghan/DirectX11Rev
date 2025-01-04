#pragma once
#include "FlowNode.h"
#include "ScratchTextureAsset.h"

class IBLMaterialAssetCreateFlowNode : public FlowNode<
	ScratchTextureAsset, 
	ScratchTextureAsset, 
	ScratchTextureAsset,
	ScratchTextureAsset,
	float,
	float
>
{
public:
	IBLMaterialAssetCreateFlowNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~IBLMaterialAssetCreateFlowNode() override = default;

protected:
	virtual void ExecuteImpl() override;
};

