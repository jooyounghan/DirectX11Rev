#include "AModelComponent.h"

using namespace DirectX;

AModelComponent::AModelComponent(
	const uint32_t& componentID,
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale
)
	: AComponent(componentID, position, rotation, scale)
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
