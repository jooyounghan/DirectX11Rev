#pragma once
#include "FlowNode.h"
#include "ScratchTextureAsset.h"

class AssetManager;

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
		ID3D11Device** deviceAddress,
		ID3D11DeviceContext** deviceContextAddress,
		AssetManager* assetManager
	);
	~IBLMaterialAssetCreateFlowNode() override = default;

protected:
	ID3D11Device** m_deviceAddressCached = nullptr;
	ID3D11DeviceContext** m_deviceContextAddressCached = nullptr;
	AssetManager* m_assetManagerCached = nullptr;

protected:
	virtual void ExecuteImpl() override;
};

