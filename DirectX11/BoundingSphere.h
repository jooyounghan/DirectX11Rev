#pragma once
#include "ABoundingComponent.h"
 
class OrientedBoundingBox;

class BoundingSphere : public ABoundingComponent
{
public:
	BoundingSphere(
		GraphicsPipeline* GraphicsPipelineInstances,
		AssetManager* AssetManagerInstance,
		const float& RadiusIn
	);
	virtual ~BoundingSphere();

protected:
	XMVECTOR Center;
	float Radius;
	MakeGetter(Center);
	MakeGetter(Radius);


public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) override;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	bool IsInsideOrOnPlane(const Plane& PlaneIn);
	bool IsOverlappedWithSphere(BoundingSphere* SphereIn);
};

