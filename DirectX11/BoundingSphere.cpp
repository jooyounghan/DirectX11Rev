#include "BoundingSphere.h"
#include "AssetManager.h"
#include "Debugable.h"
#include "CollisionVisitor.h"

using namespace std;
using namespace DirectX;

BoundingSphere::BoundingSphere(
	GraphicsPipeline* GraphicsPipelineInstances,
	AssetManager* AssetManagerInstance,
	const float& RadiusIn
)
	: ABoundingComponent(GraphicsPipelineInstances, AssetManagerInstance), Radius(RadiusIn)
{
	static size_t BoundingSphereCount = 0;
	BoundingSphereCount++;
	ObjectName = "Bounding Sphere " + to_string(BoundingSphereCount);

	AutoZeroMemory(Center);
	Scale.x = RadiusIn;
	Scale.y = RadiusIn;
	Scale.z = RadiusIn;

	DebugObject = AssetManagerInstance->GetDebugObject(EDebugObjectType::Sphere);
	DebugObject->UpdateColor(XMVectorSet(1.f, 0.f, 0.f, 1.f), DeviceContextCached);
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
	IsCollided = CollisionVisitor->Visit(this);
	return IsCollided;
}


void BoundingSphere::UpdateObject(const float& DeltaTimeIn)
{
	static float Test = 0;
	Test += DeltaTimeIn;
	Position.x = 400 * sin(0.5f * 3.141592f * Test);
	Position.y = 400 * sin(0.5f * 3.141592f * Test);

	ABoundingComponent::UpdateObject(DeltaTimeIn);

	XMVECTOR Scaling;
	XMVECTOR RotationQuat;
	XMMatrixDecompose(&Scaling, &RotationQuat, &Center, GetTransformation());

	Radius = Scale.x;
}

bool BoundingSphere::IsInsideOrOnPlane(const Plane& PlaneIn)
{
	XMVECTOR FromPlaneToSphere = Center - PlaneIn.Point;
	float DistanceFromPlaneToSphereCenter = XMVectorGetX(XMVector3Dot(FromPlaneToSphere, PlaneIn.Normal));
	return DistanceFromPlaneToSphereCenter >= -Radius;
}

bool BoundingSphere::IsOverlappedWithSphere(BoundingSphere* SphereIn)
{
	float Distance = XMVectorGetX(XMVector3Length(Center - SphereIn->Center));
	return Distance <= (Radius + SphereIn->Radius);
}
