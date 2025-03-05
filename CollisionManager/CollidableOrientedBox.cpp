#include "CollidableOrientedBox.h"
#include "CollisionVisitor.h"

using namespace DirectX;

bool CollidableOrientedBox::Accept(ICollisionVisitor& collisionVisitor) const
{
	return collisionVisitor.Visit(this);
}

bool CollidableOrientedBox::IsIntersectBoundingBox(const DirectX::BoundingBox& boundingBox) const
{
    return boundingBox.Intersects(*this);
}

bool CollidableOrientedBox::IsContainedByBoundingBox(const DirectX::BoundingBox& boundingBox) const
{
    return boundingBox.Contains(*this) == ContainmentType::CONTAINS;
}

DirectX::BoundingBox CollidableOrientedBox::GetBoundingBox(const float& margin) const
{
    return BoundingBox(Center, XMFLOAT3(Extents.x + margin, Extents.y + margin, Extents.z + margin));
}

void CollidableOrientedBox::SetBoundingProperties(
    const DirectX::XMVECTOR& center,
    const DirectX::XMFLOAT3& extents,
    const DirectX::XMVECTOR& rotationQuaternion
)
{
    XMStoreFloat3(&Center, center);
    Extents = extents;
    XMStoreFloat4(&Orientation, rotationQuaternion);
}
