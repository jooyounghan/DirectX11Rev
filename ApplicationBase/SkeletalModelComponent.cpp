#include "SkeletalModelComponent.h"

using namespace std;
using namespace DirectX;

SkeletalModelComponent::SkeletalModelComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale
)
	: AModelComponent(componentName, componentID, position, rotation, scale)
{
}

void SkeletalModelComponent::SetSkeletalMeshAsset(const SkeletalMeshAsset* skeletalMeshAsset)
{
	m_skeletalMeshAsset = skeletalMeshAsset;
	m_selectedMaterials = m_skeletalMeshAsset->GetDefaultMaterials();
}

void SkeletalModelComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
