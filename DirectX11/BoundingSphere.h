#pragma once
#include "IIntersectable.h"
#include "AttachableObject.h"
#include "IRenderable.h"
#include "Debugable.h"
#include <memory>

class GraphicsPipeline;

class BoundingSphere : public IIntersectable, public AttachableObject, public IRenderable
{
public:
	BoundingSphere(
		GraphicsPipeline* GraphicsPipelineInstances,
		const float& RadiusIn, Debugable* DebugObjectIn
	);
	virtual ~BoundingSphere();

protected:
	SPosition3D ParentPosition;

protected:
	Debugable* DebugObject = nullptr;

public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) override;
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject) override;

public:
	virtual void Render(PSOObject* PSOObjectIn) override;
};

