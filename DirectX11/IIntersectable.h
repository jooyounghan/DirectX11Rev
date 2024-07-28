#pragma once
#include "IObject.h"
#include <DirectXCollision.h>

class Ray;

class IIntersectable : public IObject
{
public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) = 0;
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParnetObject) = 0;
};

