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
	const XMVECTOR rotationQuaternion = GetAbsoluteRotationQuaternion();
	SetBoundingProperties(m_localPosition, extends, rotationQuaternion);
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

	SetBoundingProperties(m_absolutePosition, Extents, GetAbsoluteRotationQuaternion());
}

void OrientedBoxCollisionComponent::UpdateComponentTransformation()
{
	m_transformation.m_transformation = XMMatrixScalingFromVector(XMLoadFloat3(&Extents)) * GetAbsoluteTransformation();
	m_transformation.m_invTransformation = XMMatrixInverse(nullptr, m_transformation.m_transformation);
	m_transformation.m_transformation = XMMatrixTranspose(m_transformation.m_transformation);
}

void OrientedBoxCollisionComponent::SetCollisionOption(ICollisionOption* collisionOption)
{
	if (m_collisionOption) m_collisionOption->RemoveBVHImpl(this);
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
