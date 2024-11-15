#pragma once
#include "Ray.h"
#include "CollisionVisitor.h"

class IIntersectable
{
public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) = 0;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) = 0;
};

