#pragma once

struct Ray;

class IIntersectable
{
public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) = 0;
};

