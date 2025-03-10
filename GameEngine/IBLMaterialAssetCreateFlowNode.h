#pragma once
#include "FlowNode.h"
#include "ScratchTextureAsset.h"

class IBLMaterialAssetCreateFlowNode : public FlowNode<
	std::string,
	std::string,
	ScratchTextureAsset*, 
	ScratchTextureAsset*, 
	ScratchTextureAsset*,
	ScratchTextureAsset*,
	float,
	float
>
{
public:
	IBLMaterialAssetCreateFlowNode(
		const ImVec2& leftTop, const float& radius, 
		const ImVec2& referencedOrigin, 
		ID3D11Device* const* deviceAddress,
		ID3D11DeviceContext* const* deviceContextAddress
	);
	~IBLMaterialAssetCreateFlowNode() override = default;

protected:
	ID3D11Device* const* m_deviceAddressCached = nullptr;
	ID3D11DeviceContext* const* m_deviceContextAddressCached = nullptr;

protected:
	virtual void ExecuteImpl() override;
};

