#include "SphereCollisionComponent.h"
#include "IComponentVisitor.h"
#include "ICollisionOption.h"

using namespace std;
using namespace DirectX;

SphereCollisionComponent::SphereCollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const float& radius
)
	: ACollisionComponent(componentName, componentID, localPosition, XMFLOAT3(0.f, 0.f, 0.f))
{
	SetBoundingProperties(localPosition, radius);;
}

SphereCollisionComponent::~SphereCollisionComponent()
{
	m_collisionOption->RemoveBVHImpl(this);
}

void SphereCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void SphereCollisionComponent::UpdateAbsoluteEntities()
{
	AComponent::UpdateAbsoluteEntities();

	XMFLOAT3 center;
	XMStoreFloat3(&center, m_absolutePosition);

	XMFLOAT4 orientation = GetAbsoluteRotationQuaternion();
	SetBoundingProperties(center, Radius);

}

void SphereCollisionComponent::SetCollisionOption(ICollisionOption* collisionOption)
{
	ACollisionComponent::SetCollisionOption(collisionOption);
	m_collisionOption->InsertBVHImpl(this);
}

void SphereCollisionComponent::UpdateBoundingVolumeHierachy()
{
	m_collisionOption->UpdateBVHImpl(this);
}

void SphereCollisionComponent::OnCollide(ICollisionAcceptor* accpetor)
{
	m_collisionOption->OnCollideImpl(this);
}
