#include "SkeletalModelComponent.h"

using namespace std;
using namespace DirectX;

SkeletalModelComponent::SkeletalModelComponent(
	const uint64_t& componentID, DirectX::XMFLOAT3 position,
	const SkeletalMeshAsset* const skeletalMeshAsset
)
	: AModelComponent(componentID, position), m_skeletalMeshAsset(skeletalMeshAsset)
{
	m_selectedMaterials = m_skeletalMeshAsset->GetDefaultMaterials();
}
