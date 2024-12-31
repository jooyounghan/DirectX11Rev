#include "StaticMeshComponent.h"
#include "StaticMeshAsset.h"

using namespace std;
using namespace DirectX;

StaticMeshComponent::StaticMeshComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale
)
	: AMeshComponent(componentName, componentID, position, rotation, scale)
{
}

void StaticMeshComponent::UpdateStaticMeshAsset(IStaticMeshProvider& provider)
{
	m_staticMeshAsset = provider.GetStaticMeshAsset(m_staticMeshName);
	if (m_staticMeshAsset != nullptr)
	{
		m_selectedModelMaterials = m_staticMeshAsset->GetDefaultMaterials();
	}
}

void StaticMeshComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
