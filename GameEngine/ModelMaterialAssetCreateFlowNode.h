#pragma once
#include "FlowNode.h"
#include "BaseTextureAsset.h"

#include <DirectXMath.h>

class AssetManager;

class ModelMaterialAssetCreateFlowNode : public FlowNode<
	std::string,
	std::string,
	BaseTextureAsset*,
	BaseTextureAsset*,
	BaseTextureAsset*,
	BaseTextureAsset*,
	BaseTextureAsset*,
	BaseTextureAsset*,
	BaseTextureAsset*,
	BaseTextureAsset*,
	DirectX::XMFLOAT3,
	float
>
{
public:
	ModelMaterialAssetCreateFlowNode(
		const ImVec2& leftTop, const float& radius,
		const ImVec2& referencedOrigin,
		ID3D11Device* const* deviceAddress,
		ID3D11DeviceContext* const* deviceContextAddress,
		AssetManager* assetManager
	);
	~ModelMaterialAssetCreateFlowNode() override = default;

protected:
	ID3D11Device* const* m_deviceAddressCached = nullptr;
	ID3D11DeviceContext* const* m_deviceContextAddressCached = nullptr;
	AssetManager* m_assetManagerCached = nullptr;

protected:
	virtual void ExecuteImpl() override;
};

