#include "AModelComponent.h"

void AModelComponent::UpdateMaterial(const size_t& materialIdx, ModelMaterialAsset* materialAsset)
{
	if (m_selectedMaterials.size() > materialIdx)
	{
		m_selectedMaterials[materialIdx] = materialAsset;
	}
	else
	{
		throw std::exception();
	}
}
