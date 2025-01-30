#pragma once
#include "FlowNode.h"
#include "BaseTextureAsset.h"

#include <DirectXMath.h>

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
		ID3D11DeviceContext* const* deviceContextAddress
	);
	~ModelMaterialAssetCreateFlowNode() override = default;

protected:
	ID3D11Device* const* m_deviceAddressCached = nullptr;
	ID3D11DeviceContext* const* m_deviceContextAddressCached = nullptr;

protected:
	virtual void ExecuteImpl() override;
};

