
#include "ContainVisitor.h"

using namespace std;
using namespace DirectX;


ContainVisitor::ContainVisitor(ICollisionAcceptor* collisionAcceptor)
	: ACollisionVisitor(collisionAcceptor)
{
}

bool ContainVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	ContainSphereVisitor containSphereVisitor(collidableSphere);
	return m_collisionAcceptor->Accept(containSphereVisitor);
}

bool ContainVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	ContainOrientedBoxVisitor containOrientedBoxVisitor(collidableOrientedBox);
	return m_collisionAcceptor->Accept(containOrientedBoxVisitor);
}

bool ContainVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	ContainFrustumVisitor containFrustumVisitor(collidableFrustum);
	return m_collisionAcceptor->Accept(containFrustumVisitor);
}

ContainSphereVisitor::ContainSphereVisitor(const ACollidableSphere* const collidableSphere)
	: ACollisionSpecifiedVisitor<ACollidableSphere>(collidableSphere)
{
}

bool ContainSphereVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	return collidableSphere->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

bool ContainSphereVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	return collidableOrientedBox->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

bool ContainSphereVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	return collidableFrustum->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

ContainOrientedBoxVisitor::ContainOrientedBoxVisitor(const ACollidableOrientedBox* const collidableOrientedBox)
	: ACollisionSpecifiedVisitor<ACollidableOrientedBox>(collidableOrientedBox)
{
}

bool ContainOrientedBoxVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	return collidableSphere->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

bool ContainOrientedBoxVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	return collidableOrientedBox->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

bool ContainOrientedBoxVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	return collidableFrustum->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

ContainFrustumVisitor::ContainFrustumVisitor(const ACollidableFrustum* const collidableFrustum)
	: ACollisionSpecifiedVisitor<ACollidableFrustum>(collidableFrustum)
{
}

bool ContainFrustumVisitor::Visit(const ACollidableSphere* const collidableSphere) const
{
	return collidableSphere->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

bool ContainFrustumVisitor::Visit(const ACollidableOrientedBox* const collidableOrientedBox) const
{
	return collidableOrientedBox->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}

bool ContainFrustumVisitor::Visit(const ACollidableFrustum* const collidableFrustum) const
{
	return collidableFrustum->Contains(*m_specifiedCollisionAccpetor) == ContainmentType::CONTAINS;
}
