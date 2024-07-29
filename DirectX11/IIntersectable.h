#pragma once
#include "IObject.h"

struct Ray
{
	SPosition3D Origin;
	SVector3D	Direction;
};

class IIntersectable : public IObject
{
public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) = 0;
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject) = 0;
};

