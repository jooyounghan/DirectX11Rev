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
	const XMFLOAT3& scale,
	const XMFLOAT3& extents
)
	: ACollisionComponent(componentName, componentID, localPosition, localAngle, scale),
	m_extents(XMVectorSet(extents.x, extents.y, extents.z, 0.f))
{
	UpdateBoundingProperty();
}

OrientedBoxCollisionComponent::~OrientedBoxCollisionComponent()
{
	m_collisionOption->RemoveBVHImpl(this);
}

const DirectX::XMMATRIX& OrientedBoxCollisionComponent::GetAbsoluteTranformation() const
{
	static XMMATRIX pureTransformation = XMMatrixIdentity();
	pureTransformation = XMMatrixScaling(
		1.f / (2.f * m_extents.m128_f32[0]),
		1.f / (2.f * m_extents.m128_f32[1]),
		1.f / (2.f * m_extents.m128_f32[2])
	) * m_transformation;
		
	return pureTransformation;
}

DirectX::XMMATRIX OrientedBoxCollisionComponent::GetLocalTransformation() const
{
	return XMMatrixAffineTransformation(
		XMVectorMultiply(m_scale, XMVectorScale(m_extents, 2.f)),
		XMQuaternionIdentity(),
		GetLocalRotationQuaternion(),
		m_localPosition
	);
}

bool OrientedBoxCollisionComponent::GetDefaultRenderable() const
{
	return false;
}

void OrientedBoxCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}

void OrientedBoxCollisionComponent::UpdateEntity()
{
	AComponent::UpdateEntity();
	UpdateBoundingProperty();
}

void OrientedBoxCollisionComponent::SetCollisionOption(ICollisionOption* collisionOption)
{
	if (m_collisionOption) m_collisionOption->RemoveBVHImpl(this);
	ACollisionComponent::SetCollisionOption(collisionOption);
	m_collisionOption->InsertBVHImpl(this);
}

void OrientedBoxCollisionComponent::UpdateBoundingVolumeHierarchy()
{
	m_collisionOption->UpdateBVHImpl(this);
}

void OrientedBoxCollisionComponent::UpdateBoundingProperty()
{
	XMStoreFloat3(&Center, m_absolutePosition);
	XMStoreFloat3(&Extents, XMVectorMultiply(m_scale, m_extents));
	XMStoreFloat4(&Orientation, GetAbsoluteRotationQuaternion());
}

void OrientedBoxCollisionComponent::OnCollide(ICollisionAcceptor* accpetor)
{
	m_collisionOption->OnCollideImpl(this);
}
