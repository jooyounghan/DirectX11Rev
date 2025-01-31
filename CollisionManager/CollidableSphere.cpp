#include "CollidableSphere.h"
#include "CollisionVisitor.h"

using namespace DirectX;

bool CollidableSphere::Accept(ICollisionVisitor& collisionVisitor) const
{
	return collisionVisitor.Visit(this);
}

bool CollidableSphere::IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const
{
	return boundingBox.Intersects(*this);
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
