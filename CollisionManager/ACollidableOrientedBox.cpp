#include "ACollidableOrientedBox.h"
#include "CollisionVisitor.h"
#include "DirectionEntity.h"

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
    const XMVECTOR orientation = XMLoadFloat4(&Orientation);

    XMFLOAT3 extents;
    XMStoreFloat3(&extents, XMVectorAbs(
        (Extents.z + margin) * XMVector3Rotate(GDefaultForward, orientation) +
        (Extents.y + margin) * XMVector3Rotate(GDefaultUp, orientation) +
        (Extents.x + margin) * XMVector3Rotate(GDefaultRight, orientation)
    ));

    return BoundingBox(Center, extents);
}