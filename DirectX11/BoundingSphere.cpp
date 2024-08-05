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
	return PlaneIn.GetSignedDistanceToPlane(Center) > -Radius;
}
