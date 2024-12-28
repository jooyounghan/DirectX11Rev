#include "AMeshComponent.h"

using namespace std;
using namespace DirectX;

AMeshComponent::AMeshComponent(
	const string& componentName,
	const uint32_t& componentID,
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale
)
	: AComponent(componentName, componentID, position, rotation, scale)
{
}

void AMeshComponent::UpdateModelMaterial(IModelMaterialProvider& provider)
{
	m_selectedModelMaterials.clear();
	for (const string& modelMaterialNames : m_modelMaterialNames)
	{
		m_selectedModelMaterials.emplace_back(provider.GetModelMaterialAsset(modelMaterialNames));
	}
}
