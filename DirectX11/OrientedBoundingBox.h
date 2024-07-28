#pragma once
#include "IIntersectable.h"

class PlacableObject;

class OrientedBoundingBox : public IIntersectable
{
public:
	OrientedBoundingBox() {};
	virtual ~OrientedBoundingBox() {};

protected:
	DirectX::BoundingOrientedBox ImplOBB;

public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut);
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParnetObject) override;
};

