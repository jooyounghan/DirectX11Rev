#include "OrientedBoundingBox.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"
#include "AssetManager.h"
#include "Debugable.h"
#include "CollisionVisitor.h"

#include <limits>

using namespace std;
using namespace DirectX;

OrientedBoundingBox::OrientedBoundingBox(
	GraphicsPipeline* GraphicsPipelineInstances,
	AssetManager* AssetManagerInstance,
	const float& HalfXIn, 
	const float& HalfYIn,
	const float& HalfZIn
)
	: ABoundingComponent(GraphicsPipelineInstances, AssetManagerInstance)
{
	static size_t BoundingOBBCount = 0;
	BoundingOBBCount++;
	ObjectName = "Bounding OBB " + to_string(BoundingOBBCount);

	AutoZeroArrayMemory(CurrentAxises);
	AutoZeroArrayMemory(HalfExtends);
	AutoZeroMemory(Center);

	Scale.x = HalfXIn;
	Scale.y = HalfYIn;
	Scale.z = HalfZIn;
	DebugObject = AssetManagerInstance->GetDebugObject(EDebugObjectType::Box);
}

OrientedBoundingBox::~OrientedBoundingBox() {}

bool OrientedBoundingBox::Intersect(Ray* RayIn, float& DistanceOut)
{
	const XMVECTOR ToCenter = Center - RayIn->Origin;

	float tMin = numeric_limits<float>::lowest();
	float tMax = numeric_limits<float>::max();

	for (size_t AxisType = 0; AxisType < Direction::NumPlaneDirection; ++AxisType)
	{
		const XMVECTOR& Axis = CurrentAxises[AxisType];
		const float HalfExtend = HalfExtends[AxisType];

		const float AxisToCenterProj = InnerProduct(Axis, ToCenter);
		const float AxisToRayDirectionCos = InnerProduct(Axis, RayIn->Direction);

		if (abs(AxisToRayDirectionCos) > 1E-6)
		{
			float t1 = (AxisToCenterProj + HalfExtend) / AxisToRayDirectionCos;
			float t2 = (AxisToCenterProj - HalfExtend) / AxisToRayDirectionCos;

			if (t1 > t2) swap(t1, t2);

			if (t1 > tMin) tMin = t1;
			if (t2 < tMax) tMax = t2;
			if (tMin > tMax) return false;
			if (tMax < 0) return false;
		}
		else if (-AxisToCenterProj - HalfExtend > 0 || -AxisToCenterProj + HalfExtend < 0) return false;
		else;
	}

	DistanceOut = tMin > 0 ? tMin : tMax;
	return true;
}

bool OrientedBoundingBox::AcceptCollision(ICollisionVisitor* CollisionVisitor)
{
	IsCollided = CollisionVisitor->Visit(this);
	return IsCollided;
}

void OrientedBoundingBox::UpdateObject(const float& DeltaTimeIn)
{
	static float SpentTime = 0;
	SpentTime += DeltaTimeIn;

	Position.x = 800.f * sin(XMConvertToRadians(90.f * SpentTime));
	Angle.Yaw += 180.f * DeltaTimeIn;
	Angle.Roll += 180.f * DeltaTimeIn;
	Angle.Pitch += 180.f * DeltaTimeIn;
	ABoundingComponent::UpdateObject(DeltaTimeIn);

	XMVECTOR Scaling;
	XMVECTOR RotationQuat;
	XMMatrixDecompose(&Scaling, &RotationQuat, &Center, GetTransformation());

	CurrentAxises[Direction::PlaneRight] = XMVector3Rotate(Direction::GDefaultRight, RotationQuat);
	CurrentAxises[Direction::PlaneUp] = XMVector3Rotate(Direction::GDefaultUp, RotationQuat);
	CurrentAxises[Direction::PlaneForward] = XMVector3Rotate(Direction::GDefaultForward, RotationQuat);

	HalfExtends[Direction::PlaneRight] = Scale.x;
	HalfExtends[Direction::PlaneUp] = Scale.y;
	HalfExtends[Direction::PlaneForward] = Scale.z;
}

bool OrientedBoundingBox::IsInsideOrOnPlane(const Plane& PlaneIn)
{
	XMVECTOR FromPlaneToSphere = Center - PlaneIn.Point;
	const float DistanceFromPlaneToSphereCenter = XMVectorGetX(XMVector3Dot(FromPlaneToSphere, PlaneIn.Normal));
	const float Radius =
		HalfExtends[Direction::EPlaneDirection::PlaneRight] * abs(PlaneIn.Normal.m128_f32[0]) +
		HalfExtends[Direction::EPlaneDirection::PlaneUp] * abs(PlaneIn.Normal.m128_f32[1]) +
		HalfExtends[Direction::EPlaneDirection::PlaneForward] * abs(PlaneIn.Normal.m128_f32[2]);

	return DistanceFromPlaneToSphereCenter >= -Radius;
}

bool OrientedBoundingBox::IsOverlappedWithOBB(OrientedBoundingBox* OBBIn)
{
	if (IsOverlappedWithOBBByParellelEach(OBBIn))
	{
		if (IsOverlappedWithOBBNormalBoth(OBBIn))
		{
			return true;
		}
	}
	return false;
}

float OrientedBoundingBox::GetHalfExtendsLengthToAxis(const XMVECTOR& AxisIn)
{
	float HalfExtendsToAxisLength = 0.f;
	for (size_t i = 0; i < Direction::NumPlaneDirection; ++i)
	{
		XMVECTOR OBB1HalfExtendVector = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		OBB1HalfExtendVector.m128_f32[i] = HalfExtends[i];

		HalfExtendsToAxisLength += fabs(XMVectorGetX(XMVector3Dot(OBB1HalfExtendVector, AxisIn)));
	}
	return HalfExtendsToAxisLength;
}

bool OrientedBoundingBox::IsOverlappedWithOBBByParellelEach(OrientedBoundingBox* OBBIn)
{
	const size_t OBBCounts = 2;
	const OrientedBoundingBox* OBBs[OBBCounts]{ this, OBBIn };
	const XMVECTOR FromCenterToCenter = OBBIn->Center - Center;

	for (size_t OBBIdx = 0; OBBIdx < OBBCounts; ++OBBIdx)
	{
		const OrientedBoundingBox* CurrentOBB = OBBs[OBBIdx];
		for (size_t AxisIdx = 0; AxisIdx < Direction::NumPlaneDirection; ++AxisIdx)
		{
			const XMVECTOR& CurrentAxis = CurrentOBB->CurrentAxises[AxisIdx];

			float FromCenterToCenterLength = fabs(XMVectorGetX(XMVector3Dot(FromCenterToCenter, CurrentAxis)));
			float OBB1ToAxisLength = GetHalfExtendsLengthToAxis(CurrentAxis);
			float OBB2ToAxisLength = OBBIn->GetHalfExtendsLengthToAxis(CurrentAxis);
			if (FromCenterToCenterLength > OBB1ToAxisLength + OBB2ToAxisLength)
			{
				return false;
			}
		}
	}
	return true;
}

bool OrientedBoundingBox::IsOverlappedWithOBBNormalBoth(OrientedBoundingBox* OBBIn)
{
	const size_t OBBCounts = 2;
	const OrientedBoundingBox* OBBs[OBBCounts]{ this, OBBIn };

	const XMVECTOR FromCenterToCenter = OBBIn->Center - Center;

	for (size_t i = 0; i < Direction::NumPlaneDirection; ++i)
	{
		for (size_t j = 0; j < Direction::NumPlaneDirection; ++j)
		{
			const XMVECTOR CrossAxis = XMVector3Normalize(XMVector3Cross(CurrentAxises[i], OBBIn->CurrentAxises[j]));

			float FromCenterToCenterLength = fabs(XMVectorGetX(XMVector3Dot(FromCenterToCenter, CrossAxis)));
			float OBB1ToAxisLength = GetHalfExtendsLengthToAxis(CrossAxis);
			float OBB2ToAxisLength = OBBIn->GetHalfExtendsLengthToAxis(CrossAxis);

			if (FromCenterToCenterLength > OBB1ToAxisLength + OBB2ToAxisLength)
			{
				return false;
			}
		}
	}
	return true;
}
