#include "SkeletalMeshComponent.h"
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace DirectX;

SkeletalMeshComponent::SkeletalMeshComponent(
	const string& componentName,
	const uint32_t& componentID, 
	const XMFLOAT3& position,
	const XMFLOAT3& rotation,
	const XMFLOAT3& scale
)
	: AMeshComponent(componentName, componentID, position, rotation, scale)
{
}

void SkeletalMeshComponent::SetAnimationPlayer(AnimationPlayer* animationPlayer)
{
	if (m_animationPlayer) delete m_animationPlayer;
	m_animationPlayer = animationPlayer;
}

void SkeletalMeshComponent::UpdateSkeletalMeshAsset(ISkeletalMeshProvider& provider)
{
	m_skeletalMeshAsset = provider.GetSkeletalMeshAsset(m_skeletalMeshName);
}

void SkeletalMeshComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
