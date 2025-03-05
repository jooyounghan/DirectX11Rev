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
    static const XMVECTOR forwardVector = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    static const XMVECTOR rightVector = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    static const XMVECTOR upVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    
    const XMVECTOR orientation = XMLoadFloat4(&Orientation);
    
    
    XMFLOAT3 extents;
    XMStoreFloat3(&extents, XMVectorAbs(
        (Extents.z + margin)* XMVector3Rotate(forwardVector, orientation) +
        (Extents.y + margin) * XMVector3Rotate(upVector, orientation) +
        (Extents.x + margin) * XMVector3Rotate(rightVector, orientation)
    ));

    return BoundingBox(Center, extents);
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
