#pragma once
#include "IIntersectable.h"
#include "RelativePlaceableObject.h"

class GraphicsPipeline;
class AssetManager;
class Debugable;

class ABoundingComponent : public IIntersectable, public RelativePlaceableObject
{
public:
	ABoundingComponent(
		GraphicsPipeline* GraphicsPipelineInstances,
		AssetManager* AssetManagerInstance
	);
	virtual ~ABoundingComponent();

protected:
	Debugable* DebugObject = nullptr;

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) = 0; 

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) = 0;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Render(PSOObject* PSOObjectIn) override final;
};

