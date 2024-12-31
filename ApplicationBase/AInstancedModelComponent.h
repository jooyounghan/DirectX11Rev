#pragma once
#include "AComponent.h"
#include "ModelMaterialAsset.h"

class AInstancedModelComponent : public AComponent
{
public:
	AInstancedModelComponent(const uint32_t& componentID, DirectX::XMFLOAT3 position);
	~AInstancedModelComponent() override = default;

protected:
	std::vector<ModelMaterialAsset*> m_selectedMaterials;

public:
	void UpdateMaterial(const size_t& materialIdx, ModelMaterialAsset* materialAsset);
};

