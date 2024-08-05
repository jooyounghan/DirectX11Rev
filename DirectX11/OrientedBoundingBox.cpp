#include "OrientedBoundingBox.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"
#include "AssetManager.h"
#include "Debugable.h"
#include "CollisionVisitor.h"

#include <limits>

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
	AutoZeroArrayMemory(CurrentAxis);
	AutoZeroMemory(Center);

	Scale.x = HalfXIn;
	Scale.y = HalfYIn;
	Scale.z = HalfZIn;
	DebugObject = AssetManagerInstance->GetDebugObject(EDebugObjectType::Box);
}

OrientedBoundingBox::~OrientedBoundingBox() {}

bool OrientedBoundingBox::Intersect(Ray* RayIn, float& DistanceOut)
{
	const size_t AxisNum = Direction::EDirection::NumDirection;
	const float OBBHalfExtents[AxisNum] = {
		Scale.x,
		Scale.y,
		Scale.z,
	};

	const XMVECTOR ToCenter = Center - RayIn->Origin;

	float tMin = std::numeric_limits<float>::lowest();
	float tMax = std::numeric_limits<float>::max();

	for (size_t AxisType = 0; AxisType < AxisNum; ++AxisType)
	{
		const XMVECTOR& Axis = CurrentAxis[AxisType];
		const float HalfExtend = OBBHalfExtents[AxisType];

		const float AxisToCenterProj = InnerProduct(Axis, ToCenter);
		const float AxisToRayDirectionCos = InnerProduct(Axis, RayIn->Direction);

		if (abs(AxisToRayDirectionCos) > 1E-6)
		{
			float t1 = (AxisToCenterProj + HalfExtend) / AxisToRayDirectionCos;
			float t2 = (AxisToCenterProj - HalfExtend) / AxisToRayDirectionCos;

			if (t1 > t2) std::swap(t1, t2);

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
	return CollisionVisitor->Visit(this);
}

void OrientedBoundingBox::UpdateObject(const float& DeltaTimeIn)
{
	Angle.Yaw += 20.f * DeltaTimeIn;
	Angle.Roll += 40.f * DeltaTimeIn;
	ABoundingComponent::UpdateObject(DeltaTimeIn);

	XMVECTOR Scaling;
	XMVECTOR RotationQuat;
	XMMatrixDecompose(&Scaling, &RotationQuat, &Center, GetTransformation());

	CurrentAxis[Direction::EDirection::Right] = XMVector3Rotate(Direction::GDefaultRight, RotationQuat);
	CurrentAxis[Direction::EDirection::Up] = XMVector3Rotate(Direction::GDefaultUp, RotationQuat);
	CurrentAxis[Direction::EDirection::Forward] = XMVector3Rotate(Direction::GDefaultForward, RotationQuat);
}
