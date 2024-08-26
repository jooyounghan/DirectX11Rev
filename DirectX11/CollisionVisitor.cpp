#include "CollisionVisitor.h"

#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "BoundingFrustum.h"

#include "GlobalVariable.h"

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
	return t1->IsOverlappedWithSphere(t2);
}

bool CollisionLogic::CheckCollision(BoundingSphere* t1, OrientedBoundingBox* t2)
{
	const XMVECTOR* CurrentAxises = t2->GetCurrentAxises();
	const float* HalfExtends = t2->GetHalfExtends();
	const XMVECTOR& OBBCenter = t2->GetCenter();
	const XMVECTOR& SphereCenter = t1->GetCenter();
	const float SphereRadius = t1->GetScaledRadius();

	const XMVECTOR FromOBBToSphereCenter = SphereCenter - OBBCenter;

	for (size_t AxisType = 0; AxisType < Direction::NumPlaneDirection; ++AxisType)
	{
		const XMVECTOR& OrientedAxis = CurrentAxises[AxisType];
		const float& OrientedHalfExtend = HalfExtends[AxisType];
		const float DistanceToOrietnedAxis = abs(XMVectorGetX(XMVector3Dot(FromOBBToSphereCenter, OrientedAxis)));
		
		if (DistanceToOrietnedAxis > (SphereRadius + OrientedHalfExtend))
		{
			return false;
		}
	}
	return true;
}

bool CollisionLogic::CheckCollision(BoundingSphere* t1, BoundingFrustum* t2)
{
	bool result = true;
	for (size_t FrustumPlaneIdx = 0; FrustumPlaneIdx < Direction::EFrstumDirection::NumFrustumDirection; ++FrustumPlaneIdx)
	{
		result = result && (t1->IsInsideOrOnPlane(t2->GetFrustumPlane((Direction::EFrstumDirection)FrustumPlaneIdx)));
	}
	if (result)
	{
		bool test = true;
	}

	return result;
}

bool CollisionLogic::CheckCollision(OrientedBoundingBox* t1, BoundingSphere* t2)
{
	return CollisionLogic::CheckCollision(t2, t1);
}

bool CollisionLogic::CheckCollision(OrientedBoundingBox* t1, OrientedBoundingBox* t2)
{
	return t1->IsOverlappedWithOBB(t2);
}

bool CollisionLogic::CheckCollision(OrientedBoundingBox* t1, BoundingFrustum* t2)
{
	bool result = true;
	for (size_t FrustumPlaneIdx = 0; FrustumPlaneIdx < Direction::EFrstumDirection::NumFrustumDirection; ++FrustumPlaneIdx)
	{
		result = result && (t1->IsInsideOrOnPlane(t2->GetFrustumPlane((Direction::EFrstumDirection)FrustumPlaneIdx)));
	}
	return result;
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
