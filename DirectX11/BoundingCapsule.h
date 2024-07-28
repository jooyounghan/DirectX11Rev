#pragma once
#include "IIntersectable.h"
class BoundingCapsule : public IIntersectable
{
public:
	BoundingCapsule() {};
	virtual ~BoundingCapsule() {};

public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut)
	{
		return false;
	}
};

