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
	SetBoundingProperties(m_localPosition, radius);
}

SphereCollisionComponent::~SphereCollisionComponent()
{
	m_collisionOption->RemoveBVHImpl(this);
}

bool SphereCollisionComponent::GetDefaultRenderable()
{
	return false;
}

void SphereCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void SphereCollisionComponent::UpdateAbsoluteEntities()
{
	AComponent::UpdateAbsoluteEntities();
	SetBoundingProperties(m_absolutePosition, Radius);
}

void SphereCollisionComponent::UpdateComponentTransformation()
{
	m_transformation.m_transformation = XMMatrixScaling(Radius, Radius, Radius) * GetAbsoluteTransformation();
	m_transformation.m_invTransformation = XMMatrixInverse(nullptr, m_transformation.m_transformation);
	m_transformation.m_transformation = XMMatrixTranspose(m_transformation.m_transformation);
}

void SphereCollisionComponent::SetCollisionOption(ICollisionOption* collisionOption)
{
	if (m_collisionOption) m_collisionOption->RemoveBVHImpl(this);
	ACollisionComponent::SetCollisionOption(collisionOption);
	if (m_collisionOption) m_collisionOption->InsertBVHImpl(this);
}

void SphereCollisionComponent::UpdateBoundingVolumeHierarchy()
{
	m_collisionOption->UpdateBVHImpl(this);
}

void SphereCollisionComponent::OnCollide(ICollisionAcceptor* accpetor)
{
	m_collisionOption->OnCollideImpl(this);
}
