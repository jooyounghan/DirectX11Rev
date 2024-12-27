#include "AModelComponent.h"

using namespace std;
using namespace DirectX;

AModelComponent::AModelComponent(
	const string& componentName,
	const uint32_t& componentID,
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale
)
	: AComponent(componentName, componentID, position, rotation, scale)
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
		throw exception();
	}
}
