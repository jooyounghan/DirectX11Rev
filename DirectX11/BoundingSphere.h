#pragma once
#include "IIntersectable.h"
#include "RelativePlaceableObject.h"

class GraphicsPipeline;
class Debugable;
 
class BoundingSphere : public IIntersectable, public RelativePlaceableObject
{
public:
	BoundingSphere(
		GraphicsPipeline* GraphicsPipelineInstances,
		const float& RadiusIn, Debugable* DebugObjectIn
	);
	virtual ~BoundingSphere();

protected:
	Debugable* DebugObject = nullptr;

public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) override;

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Render(PSOObject* PSOObjectIn) override;
};

