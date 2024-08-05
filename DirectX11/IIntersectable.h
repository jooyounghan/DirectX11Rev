#pragma once

struct Ray;
class ICollisionVisitor;

class IIntersectable
{
public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) = 0;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) = 0;
};

