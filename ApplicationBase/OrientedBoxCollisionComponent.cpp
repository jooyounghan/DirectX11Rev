#include "OrientedBoxCollisionComponent.h"
#include "IComponentVisitor.h"
#include "ICollisionOption.h"

using namespace std;
using namespace DirectX;

OrientedBoxCollisionComponent::OrientedBoxCollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const XMFLOAT3& localAngle, 
	const XMFLOAT3& extends
)
	: ACollisionComponent(componentName, componentID, localPosition, localAngle)
{
	XMFLOAT4 rotationQuaternion = GetAbsoluteRotationQuaternion();
	SetBoundingProperties(localPosition, extends, rotationQuaternion);
}

OrientedBoxCollisionComponent::~OrientedBoxCollisionComponent()
{
	m_collisionOption->RemoveBVHImpl(this);
}

void OrientedBoxCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void OrientedBoxCollisionComponent::UpdateAbsoluteEntities()
{
	AComponent::UpdateAbsoluteEntities();

	XMFLOAT3 center;
	XMStoreFloat3(&center, m_absolutePosition);

	XMFLOAT4 orientation = GetAbsoluteRotationQuaternion();
	SetBoundingProperties(center, Extents, orientation);

}

void OrientedBoxCollisionComponent::SetCollisionOption(ICollisionOption* collisionOption)
{
	ACollisionComponent::SetCollisionOption(collisionOption);
	m_collisionOption->InsertBVHImpl(this);
}

void OrientedBoxCollisionComponent::UpdateBoundingVolumeHierachy()
{
	m_collisionOption->UpdateBVHImpl(this);
}

void OrientedBoxCollisionComponent::OnCollide(ICollisionAcceptor* accpetor)
{
	m_collisionOption->OnCollideImpl(this);
}
