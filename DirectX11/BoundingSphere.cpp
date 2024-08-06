#include "BoundingSphere.h"
#include "AssetManager.h"
#include "Debugable.h"
#include "CollisionVisitor.h"

using namespace DirectX;

BoundingSphere::BoundingSphere(
	GraphicsPipeline* GraphicsPipelineInstances,
	AssetManager* AssetManagerInstance,
	const float& RadiusIn
)
	: ABoundingComponent(GraphicsPipelineInstances, AssetManagerInstance), Radius(RadiusIn)
{
	AutoZeroMemory(Center);
	Scale.x = RadiusIn;
	Scale.y = RadiusIn;
	Scale.z = RadiusIn;

	DebugObject = AssetManagerInstance->GetDebugObject(EDebugObjectType::Sphere);
}

BoundingSphere::~BoundingSphere()
{

}

bool BoundingSphere::Intersect(Ray* RayIn, float& DistanceOut)
{
	const XMVECTOR ToCenter = RayIn->Origin - Center;

	float b = InnerProduct(RayIn->Direction, ToCenter);
	float c = InnerProduct(ToCenter, ToCenter) - Radius * Radius;

	float determinant = b * b - c;
	if (determinant < 0) 
	{ 
		return false; 
	}
	else
	{
		if (b > 0)
		{
			return false;
		}
		else
		{
			if (determinant == 0)
			{
				DistanceOut = -b;
			}
			else
			{
				float t1 = -b - sqrtf(b * b - c);
				float t2 = -b + sqrtf(b * b - c);
				DistanceOut = t1 < t2 ? t1 : t2;
			}
			return true;
		}
	}
}

bool BoundingSphere::AcceptCollision(ICollisionVisitor* CollisionVisitor)
{
	return CollisionVisitor->Visit(this);
}


void BoundingSphere::UpdateObject(const float& DeltaTimeIn)
{
	ABoundingComponent::UpdateObject(DeltaTimeIn);

	XMVECTOR Scaling;
	XMVECTOR RotationQuat;
	XMMatrixDecompose(&Scaling, &RotationQuat, &Center, GetTransformation());

	Radius = Scale.x;
}

bool BoundingSphere::IsOnOrForwardPlane(const Plane& PlaneIn)
{
	XMVECTOR vectorFromPlaneToSphere = Center - PlaneIn.Point;

	// 평면의 법선 벡터와 위에서 구한 벡터의 내적을 구합니다.
	float distanceFromPlaneToSphereCenter = XMVectorGetX(XMVector3Dot(vectorFromPlaneToSphere, PlaneIn.Normal));


	// 구의 중심이 평면 위에서 떨어진 거리가 구의 반지름보다 작거나 같으면 true를 반환합니다.
	return distanceFromPlaneToSphereCenter > -Radius;
}
