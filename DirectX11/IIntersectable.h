#pragma once
#include "DefineType.h"

struct Ray
{
	SPosition3D Origin;
	SVector3D	Direction;
};

class IIntersectable
{
public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) = 0;
};

