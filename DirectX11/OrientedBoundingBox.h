#pragma once
#include "ABoundingComponent.h"
#include "GlobalVariable.h"

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
	XMVECTOR CurrentAxises[Direction::NumPlaneDirection];
	float HalfExtends[Direction::NumPlaneDirection];
	XMVECTOR Center;
	MakeGetter(CurrentAxises);
	MakeGetter(HalfExtends);
	MakeGetter(Center);

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) override;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	bool IsInsideOrOnPlane(const Plane& PlaneIn);

public:
	bool IsOverlappedWithOBB(OrientedBoundingBox* OBBIn);

private:
	bool IsOverlappedWithOBBByParellelEach(OrientedBoundingBox* OBBIn);
	bool IsOverlappedWithOBBNormalBoth(OrientedBoundingBox* OBBIn);

private:
	float GetHalfExtendsLengthToAxis(const XMVECTOR& AxisIn);
};
