#pragma once
#include "AComponent.h"
#include "ModelMaterialAsset.h"

class AInstancedModelComponent : public AComponent
{
public:
	AInstancedModelComponent(const uint64_t& componentID, DirectX::XMFLOAT3 position);

protected:
	std::vector<ModelMaterialAsset*> m_selectedMaterials;

public:
	void UpdateMaterial(const size_t& materialIdx, ModelMaterialAsset* materialAsset);
};

