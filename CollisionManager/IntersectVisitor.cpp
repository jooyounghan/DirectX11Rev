
#include "IntersectVisitor.h"

using namespace std;
using namespace DirectX;

IntersectVisitor::IntersectVisitor(ICollisionAcceptor* collisionAcceptor)
	: ACollisionVisitor(collisionAcceptor)
{
}

bool IntersectVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	IntersectSphereVisitor intersectSphereVisitor(collidableSphere);
	return m_collisionAcceptor->Accept(intersectSphereVisitor);
}

bool IntersectVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	IntersectOrientedBoxVisitor intersectOrientedBoxVisitor(collidableOrientedBox);
	return m_collisionAcceptor->Accept(intersectOrientedBoxVisitor);
}

bool IntersectVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	IntersectFrustumVisitor intersectFrustumVisitor(collidableFrustum);
	return m_collisionAcceptor->Accept(intersectFrustumVisitor);
}

IntersectSphereVisitor::IntersectSphereVisitor(const ACollidableSphere* const collidableSphere)
	: ACollisionSpecifiedVisitor<ACollidableSphere>(collidableSphere)
{

}

bool IntersectSphereVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	return collidableSphere->Intersects(*m_specifiedCollisionAccpetor);
}

bool IntersectSphereVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	return collidableOrientedBox->Intersects(*m_specifiedCollisionAccpetor);
}

bool IntersectSphereVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	return collidableFrustum->Intersects(*m_specifiedCollisionAccpetor);
}

IntersectOrientedBoxVisitor::IntersectOrientedBoxVisitor(const ACollidableOrientedBox* const collidableOrientedBox)
	: ACollisionSpecifiedVisitor<ACollidableOrientedBox>(collidableOrientedBox)
{
}

bool IntersectOrientedBoxVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	return collidableSphere->Intersects(*m_specifiedCollisionAccpetor);
}

bool IntersectOrientedBoxVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	return collidableOrientedBox->Intersects(*m_specifiedCollisionAccpetor);
}

bool IntersectOrientedBoxVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	return collidableFrustum->Intersects(*m_specifiedCollisionAccpetor);
}


IntersectFrustumVisitor::IntersectFrustumVisitor(const ACollidableFrustum* const collidableFrustum)
	: ACollisionSpecifiedVisitor<ACollidableFrustum>(collidableFrustum)
{
}

bool IntersectFrustumVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	return collidableSphere->Intersects(*m_specifiedCollisionAccpetor);
}

bool IntersectFrustumVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	return collidableOrientedBox->Intersects(*m_specifiedCollisionAccpetor);
}

bool IntersectFrustumVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	return collidableFrustum->Intersects(*m_specifiedCollisionAccpetor);
}
