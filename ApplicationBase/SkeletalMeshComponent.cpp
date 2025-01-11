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

void SkeletalMeshComponent::UpdateSkeletalMeshAsset(ISkeletalMeshProvider& provider)
{
	m_skeletalMeshAsset = provider.GetSkeletalMeshAsset(m_skeletalMeshName);
}

void SkeletalMeshComponent::InitEntity(ID3D11Device* const device)
{
	AMeshComponent::InitEntity(device);

	if (m_skeletalMeshAsset != nullptr)
	{
		m_animationPlayer = new AnimationPlayer(m_skeletalMeshAsset->GetBoneAsset());
		m_animationPlayer->InitAnimationPlayer(device);
	}
}

void SkeletalMeshComponent::UpdateEntity(ID3D11DeviceContext* const deviceContext, const float& deltaTime)
{
	AMeshComponent::UpdateEntity(deviceContext, deltaTime);
	m_animationPlayer->UpdateAnimationPlayer(deviceContext, deltaTime);
}

void SkeletalMeshComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
