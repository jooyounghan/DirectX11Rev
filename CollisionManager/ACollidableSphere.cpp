#include "ACollidableSphere.h"
#include "CollisionVisitor.h"

using namespace DirectX;

bool ACollidableSphere::Accept(ICollisionVisitor& collisionVisitor) const
{
	return collisionVisitor.Visit(this);
}

bool ACollidableSphere::IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const
{
	return boundingBox.Intersects(*this);
}

bool ACollidableSphere::IsContainedByBoundingBox(const DirectX::BoundingBox& boundingBox) const
{
	return boundingBox.Contains(*this) == ContainmentType::CONTAINS;
}

DirectX::BoundingBox ACollidableSphere::GetBoundingBox(const float& margin) const
{
	return DirectX::BoundingBox(Center, XMFLOAT3(Radius + margin, Radius + margin, Radius + margin));
}
