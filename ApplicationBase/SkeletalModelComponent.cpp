#include "SkeletalModelComponent.h"

SkeletalModelComponent::SkeletalModelComponent(const SkeletalMeshAsset* const skeletalMeshAsset)
	: m_skeletalMeshAsset(skeletalMeshAsset)
{
	m_selectedMaterials = m_skeletalMeshAsset->GetDefaultMaterials();
}
