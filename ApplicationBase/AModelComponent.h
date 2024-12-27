#pragma once
#include "AComponent.h"
#include "ModelMaterialAsset.h"

class AModelComponent : public AComponent
{
public:
	AModelComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale	
	);

protected:
	std::vector<ModelMaterialAsset*> m_selectedMaterials;

public:
	void UpdateMaterial(const size_t& materialIdx, ModelMaterialAsset* materialAsset);
};

