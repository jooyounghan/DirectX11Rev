#pragma once
#include "IIntersectable.h"
#include "AttachableObject.h"

class BoundingSphere : public IIntersectable, public AttachableObject
{
public:
	BoundingSphere(const float& RadiusIn);
	virtual ~BoundingSphere();

protected:
	SPosition3D Center;
	float		Radius;

public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) override;
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject) override;

};

