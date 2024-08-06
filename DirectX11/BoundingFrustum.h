#pragma once
#include "ABoundingComponent.h"
#include "HeaderHelper.h"
#include "DefineType.h"

class Viewable;
class BoundingSphere;
class OrientedBoundingBox;

class BoundingFrustum : public ABoundingComponent
{
public:
	BoundingFrustum(
		GraphicsPipeline* GraphicsPipelineInstances,
		AssetManager* AssetManagerInstance,
		Viewable* ViewableInstance
	);
	virtual ~BoundingFrustum();

protected:
	Viewable* ViewableCached = nullptr;

protected:
	Plane TopFace;
	Plane BottomFace;
	Plane RightFace;
	Plane LeftFace;
	Plane FarFace;
	Plane NearFace;

public:
	MakeGetter(TopFace);
	MakeGetter(BottomFace);
	MakeGetter(RightFace);
	MakeGetter(LeftFace);
	MakeGetter(FarFace);
	MakeGetter(NearFace);

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) override;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
};

