#pragma once
#include "ABoundingComponent.h"
#include "DefineType.h"
#include "GlobalVariable.h"

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
	Plane FrustumPlanes[Direction::NumFrustumDirection];
	XMVECTOR FrustumEdgeAxises[Direction::NumFrustumEdgeAxis];

public:
	const Plane& GetFrustumPlane(Direction::EFrstumDirection DirectionIn);
	const XMVECTOR& GetFrustumEdgeAxis(Direction::EFrustumEdgeAxis EdgeAxisIn);

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) override;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void SetPropertyLength() override;
	virtual void ResetPropertyLength() override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;
};

