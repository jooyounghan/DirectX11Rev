#include "ACollidableOrientedBox.h"
#include "CollisionVisitor.h"

using namespace DirectX;

bool ACollidableOrientedBox::Accept(ICollisionVisitor& collisionVisitor) const
{
	return collisionVisitor.Visit(this);
}

bool ACollidableOrientedBox::IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const
{
    return boundingBox.Intersects(*this);
}

bool ACollidableOrientedBox::IsContainedByBoundingBox(const DirectX::BoundingBox& boundingBox) const
{
    return boundingBox.Contains(*this) == ContainmentType::CONTAINS;
}

DirectX::BoundingBox ACollidableOrientedBox::GetBoundingBox(const float& margin) const
{
    return BoundingBox(Center, XMFLOAT3(Extents.x + margin, Extents.y + margin, Extents.z + margin));
}