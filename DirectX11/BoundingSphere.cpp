#include "BoundingSphere.h"
#include "DefineUtility.h"

BoundingSphere::BoundingSphere(const float& RadiusIn)
	: Radius(RadiusIn)
{
	AutoZeroMemory(Center);
}

BoundingSphere::~BoundingSphere() 
{

}

bool BoundingSphere::Intersect(const Ray& RayIn, float& DistanceOut)
{
	float b = InnerProduct(RayIn.Direction, RayIn.Origin - Center);
	float c = InnerProduct(RayIn.Origin - Center, RayIn.Origin - Center) - Radius * Radius;

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

void BoundingSphere::UpdateObject(const float& DeltaTimeIn, IObject* ParentObject)
{
	memcpy(&Center, &(ParentObject->Position), sizeof(Center));

}
