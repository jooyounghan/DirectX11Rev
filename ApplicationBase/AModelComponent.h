#pragma once
#include "AComponent.h"
#include "ModelMaterialAsset.h"

class AModelComponent
{
public:
	AModelComponent() = default;

protected:
	std::vector<ModelMaterialAsset*> m_selectedMaterials;

public:
	void UpdateMaterial(const size_t& materialIdx, ModelMaterialAsset* materialAsset);
};

