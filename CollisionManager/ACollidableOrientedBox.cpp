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

DirectX::BoundingBox ACollidableOrientedBox::GetBoundingBox(const float& margin) const
{
    return BoundingBox(Center, XMFLOAT3(Extents.x + margin, Extents.y + margin, Extents.z + margin));
}

void ACollidableOrientedBox::SetBoundingProperties(
    const DirectX::XMVECTOR& center,
    const DirectX::XMFLOAT3& extents,
    const DirectX::XMVECTOR& rotationQuaternion
)
{
    XMStoreFloat3(&Center, center);
    Extents = extents;
    XMStoreFloat4(&Orientation, rotationQuaternion);
}
