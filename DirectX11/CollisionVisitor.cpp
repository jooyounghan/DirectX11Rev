#include "CollisionVisitor.h"

#include "BoundingSphereObject.h"
#include "OBBObject.h"
#include "BoundingFrustumObject.h"
#include "GlobalVariable.h"

CollisionVisitor::CollisionVisitor(IIntersectable* CheckObjectIn)
	: CheckObjectCahced(CheckObjectIn)
{

}

bool CollisionVisitor::Visit(BoundingSphereObject* BSInstance)
{
	CollisionVisitorWrapper<BoundingSphereObject>CollisionImpl(BSInstance);
	return CheckObjectCahced->AcceptCollision(&CollisionImpl);
}

bool CollisionVisitor::Visit(OBBObject* OBBInstance)
{
	CollisionVisitorWrapper<OBBObject>CollisionImpl(OBBInstance);
	return CheckObjectCahced->AcceptCollision(&CollisionImpl);
}

bool CollisionVisitor::Visit(BoundingFrustumObject* FrustumInstance)
{
	CollisionVisitorWrapper<BoundingFrustumObject>CollisionImpl(FrustumInstance);
	return CheckObjectCahced->AcceptCollision(&CollisionImpl);
}

bool CollisionLogic::CheckCollision(BoundingSphereObject* t1, BoundingSphereObject* t2)
{
	return t1->Intersects(*t2);
}

bool CollisionLogic::CheckCollision(BoundingSphereObject* t1, OBBObject* t2)
{
	return t1->Intersects(*t2);
}

bool CollisionLogic::CheckCollision(BoundingSphereObject* t1, BoundingFrustumObject* t2)
{
	return t1->Intersects(*t2);
}

bool CollisionLogic::CheckCollision(OBBObject* t1, BoundingSphereObject* t2)
{
	return CollisionLogic::CheckCollision(t2, t1);
}

bool CollisionLogic::CheckCollision(OBBObject* t1, OBBObject* t2)
{
	return t1->Intersects(*t2);
}

bool CollisionLogic::CheckCollision(OBBObject* t1, BoundingFrustumObject* t2)
{
	return t1->Intersects(*t2);
}

bool CollisionLogic::CheckCollision(BoundingFrustumObject* t1, BoundingSphereObject* t2)
{
	return CollisionLogic::CheckCollision(t2, t1);
}

bool CollisionLogic::CheckCollision(BoundingFrustumObject* t1, OBBObject* t2)
{
	return CollisionLogic::CheckCollision(t2, t1);
}

bool CollisionLogic::CheckCollision(BoundingFrustumObject* t1, BoundingFrustumObject* t2)
{
	return t1->Intersects(*t2);

}
