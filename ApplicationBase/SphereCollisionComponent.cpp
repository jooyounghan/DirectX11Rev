#include "SphereCollisionComponent.h"
#include "IComponentVisitor.h"
#include "ICollisionOption.h"

using namespace std;
using namespace DirectX;

SphereCollisionComponent::SphereCollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const XMFLOAT3& scale,
	const float& radius
)
	: ACollisionComponent(componentName, componentID, localPosition, XMFLOAT3(0.f, 0.f, 0.f), scale),
	m_radius(radius)
{
	UpdateBoundingProperty();
}

SphereCollisionComponent::~SphereCollisionComponent()
{
	m_collisionOption->RemoveBVHImpl(this);
}

const XMMATRIX& SphereCollisionComponent::GetAbsoluteTranformation() const
{
	static XMMATRIX pureTransformation = XMMatrixIdentity();
	pureTransformation = XMMatrixScaling(1.f / m_radius, 1.f / m_radius, 1.f / m_radius) * m_transformation;
	return pureTransformation;
}

DirectX::XMMATRIX SphereCollisionComponent::GetLocalTransformation() const
{
	return XMMatrixAffineTransformation(
		XMVectorScale(m_scale, m_radius),
		XMQuaternionIdentity(),
		GetLocalRotationQuaternion(),
		m_localPosition
	);
}

bool SphereCollisionComponent::GetDefaultRenderable() const
{
	return false;
}

void SphereCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void SphereCollisionComponent::UpdateEntity()
{
	AComponent::UpdateEntity();
	UpdateBoundingProperty();
	UpdateBoundingVolumeHierarchy();
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

void SphereCollisionComponent::UpdateBoundingProperty()
{
	XMStoreFloat3(&Center, m_absolutePosition);
	Radius = m_scale.m128_f32[0] * m_radius;
}

void SphereCollisionComponent::OnCollide(ICollisionAcceptor* accpetor)
{
	m_collisionOption->OnCollideImpl(this);
}
