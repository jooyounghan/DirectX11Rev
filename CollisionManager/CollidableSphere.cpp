#include "CollidableSphere.h"
#include "CollisionVisitor.h"
#include "BoundingVolumeNode.h"

using namespace DirectX;

bool CollidableSphere::Accept(ICollisionVisitor& collisionVisitor) const
{
	return collisionVisitor.Visit(this);
}

bool CollidableSphere::IsInBVNode(const BoundingVolumeNode* boundingVolumeNode) const
{
	return boundingVolumeNode->GetBoundingBox().Contains(*this);
}

bool CollidableSphere::IsBVNodeIn(const BoundingVolumeNode* boundingVolumeNode) const
{
	return this->Contains(boundingVolumeNode->GetBoundingBox());
}

DirectX::BoundingBox CollidableSphere::GetBoundingBox(const float& margin) const
{
	return DirectX::BoundingBox(Center, XMFLOAT3(Radius + margin, Radius + margin, Radius + margin));
}

void CollidableSphere::SetBoundingProperties(
	const DirectX::XMVECTOR& center,
	const float& radius
)
{
	XMStoreFloat3(&Center, center);
	Radius = radius;
}
