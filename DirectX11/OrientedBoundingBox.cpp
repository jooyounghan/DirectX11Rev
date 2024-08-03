#include "OrientedBoundingBox.h"
#include "GlobalVariable.h"
#include "AssetManager.h"
#include "Debugable.h"

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
	Scale.x = HalfXIn;
	Scale.y = HalfYIn;
	Scale.z = HalfZIn;
	DebugObject = AssetManagerInstance->GetDebugObject(EDebugObjectType::Box);
}

OrientedBoundingBox::~OrientedBoundingBox() {}

bool OrientedBoundingBox::Intersect(const Ray& RayIn, float& DistanceOut)
{
	XMVECTOR Scaling;
	XMVECTOR RotationQuat;
	XMVECTOR Translation;
	XMMatrixDecompose(&Scaling, &RotationQuat, &Translation, GetTranslationMatrix());

	SPosition3D Center = SPosition3D{
		Translation.m128_f32[0], Translation.m128_f32[1], Translation.m128_f32[2]
	};

	XMVECTOR BoxRotationQuat = GetRotationQuat();

	const size_t AxisNum = 3;
	const float OBBHalfExtents[AxisNum] = {
		Scale.x,
		Scale.y,
		Scale.z,
	};
	const XMVECTOR OBBAxis[AxisNum] = {
		XMVector3Rotate(Direction::GDefaultForward, BoxRotationQuat),
		XMVector3Rotate(Direction::GDefaultRight, BoxRotationQuat),
		XMVector3Rotate(Direction::GDefaultUp, BoxRotationQuat)
	};

	SVector3D ToCenter = Center - RayIn.Origin;

	float tMin = std::numeric_limits<float>::min();
	float tMax = std::numeric_limits<float>::max();

	for (size_t AxisType = 0; AxisType < AxisNum; ++AxisType)
	{
		const XMVECTOR& Axis = OBBAxis[AxisType];
		const float HalfExtend = OBBHalfExtents[AxisType];

		const float AxisToCenterProj =
			Axis.m128_f32[0] * ToCenter.x +
			Axis.m128_f32[1] * ToCenter.y +
			Axis.m128_f32[2] * ToCenter.z;

		const float AxisToRayDirectionCos =
			Axis.m128_f32[0] * RayIn.Direction.x +
			Axis.m128_f32[1] * RayIn.Direction.y +
			Axis.m128_f32[2] * RayIn.Direction.z;

		if (abs(AxisToRayDirectionCos) > 1E-6)
		{
			float t1 = (AxisToCenterProj + HalfExtend) / AxisToRayDirectionCos;
			float t2 = (AxisToCenterProj + HalfExtend) / AxisToRayDirectionCos;

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