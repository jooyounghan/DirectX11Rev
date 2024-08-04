#include "BoundingSphere.h"
#include "AssetManager.h"
#include "Debugable.h"

using namespace DirectX;

BoundingSphere::BoundingSphere(
	GraphicsPipeline* GraphicsPipelineInstances,
	AssetManager* AssetManagerInstance,
	const float& RadiusIn
)
	: ABoundingComponent(GraphicsPipelineInstances, AssetManagerInstance)
{
	Scale.x = RadiusIn;
	Scale.y = RadiusIn;
	Scale.z = RadiusIn;
	DebugObject = AssetManagerInstance->GetDebugObject(EDebugObjectType::Sphere);
}

BoundingSphere::~BoundingSphere()
{

}

bool BoundingSphere::Intersect(const Ray& RayIn, float& DistanceOut)
{
    const float& Radius = Scale.x;

	XMVECTOR Scaling;
	XMVECTOR RotationQuat;
	XMVECTOR Center;
	XMMatrixDecompose(&Scaling, &RotationQuat, &Center, GetTranslationMatrix());

	const XMVECTOR ToCenter = RayIn.Origin - Center;

	float b = InnerProduct(RayIn.Direction, ToCenter);
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