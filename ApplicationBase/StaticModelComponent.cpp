#include "StaticModelComponent.h"

using namespace std;
using namespace DirectX;

StaticModelComponent::StaticModelComponent(
	const uint32_t& componentID, 
	const XMFLOAT3& position, 
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale
)
	: AModelComponent(componentID, position, rotation, scale)
{
}

void StaticModelComponent::SetStaticMeshAsset(const StaticMeshAsset* staticMeshAsset)
{
	m_staticMeshAsset = staticMeshAsset;
	m_selectedMaterials = m_staticMeshAsset->GetDefaultMaterials();
}

void StaticModelComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
