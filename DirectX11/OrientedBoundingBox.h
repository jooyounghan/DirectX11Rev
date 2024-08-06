#pragma once
#include "ABoundingComponent.h"

class BoundingSphere;

class OrientedBoundingBox : public ABoundingComponent
{
public:
	OrientedBoundingBox(
		GraphicsPipeline* GraphicsPipelineInstances,
		AssetManager* AssetManagerInstance,
		const float& HalfXIn, 
		const float& HalfYIn, 
		const float& HalfZIn
	);
	virtual ~OrientedBoundingBox();

protected:
	XMVECTOR CurrentAxis[3];
	XMVECTOR Center;

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) override;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
};
