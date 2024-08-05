#include "CollisionVisitor.h"

#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "BoundingFrustum.h"


CollisionVisitor::CollisionVisitor(IIntersectable* CheckObjectIn)
	: CheckObjectCahced(CheckObjectIn)
{
}

bool CollisionVisitor::Visit(BoundingSphere* BSInstance)
{
	CollisionVisitorWrapper<BoundingSphere>CollisionImpl(BSInstance);
	return CheckObjectCahced->AcceptCollision(&CollisionImpl);
}

bool CollisionVisitor::Visit(OrientedBoundingBox* OBBInstance)
{
	CollisionVisitorWrapper<OrientedBoundingBox>CollisionImpl(OBBInstance);
	return CheckObjectCahced->AcceptCollision(&CollisionImpl);
}

bool CollisionVisitor::Visit(BoundingFrustum* FrustumInstance)
{
	CollisionVisitorWrapper<BoundingFrustum>CollisionImpl(FrustumInstance);
	return CheckObjectCahced->AcceptCollision(&CollisionImpl);
}

bool CollisionLogic::CheckCollision(BoundingSphere* t1, BoundingSphere* t2)
{
	return false;
}

bool CollisionLogic::CheckCollision(BoundingSphere* t1, OrientedBoundingBox* t2)
{
	return false;
}

bool CollisionLogic::CheckCollision(BoundingSphere* t1, BoundingFrustum* t2)
{
	return (t1->IsOnOrForwardPlane(t2->GetTopFace())) && 
		(t1->IsOnOrForwardPlane(t2->GetBottomFace())) && 
		(t1->IsOnOrForwardPlane(t2->GetLeftFace())) &&
		(t1->IsOnOrForwardPlane(t2->GetRightFace())) &&
		(t1->IsOnOrForwardPlane(t2->GetFarFace())) && 
		(t1->IsOnOrForwardPlane(t2->GetNearFace()));
}

bool CollisionLogic::CheckCollision(OrientedBoundingBox* t1, BoundingSphere* t2)
{
	return CollisionLogic::CheckCollision(t2, t1);
}

bool CollisionLogic::CheckCollision(OrientedBoundingBox* t1, OrientedBoundingBox* t2)
{
	return false;
}

bool CollisionLogic::CheckCollision(OrientedBoundingBox* t1, BoundingFrustum* t2)
{
	return false;
}

bool CollisionLogic::CheckCollision(BoundingFrustum* t1, BoundingSphere* t2)
{
	return CollisionLogic::CheckCollision(t2, t1);
}

bool CollisionLogic::CheckCollision(BoundingFrustum* t1, OrientedBoundingBox* t2)
{
	return CollisionLogic::CheckCollision(t2, t1);
}

bool CollisionLogic::CheckCollision(BoundingFrustum* t1, BoundingFrustum* t2)
{
	return false;
}
