#include "AModelComponent.h"

using namespace DirectX;

AModelComponent::AModelComponent(const uint64_t& componentID, XMFLOAT3 position)
	: ComponentEntity(componentID, position)
{
}

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
