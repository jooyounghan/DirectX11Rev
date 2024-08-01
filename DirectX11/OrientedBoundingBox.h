#pragma once
#include "IIntersectable.h"
#include "RelativePlaceableObject.h"

class GraphicsPipeline;
class Debugable;

class OrientedBoundingBox : public IIntersectable, public RelativePlaceableObject
{
public:
	OrientedBoundingBox(
		GraphicsPipeline* GraphicsPipelineInstances,
		const float& RadiusIn, Debugable* DebugObjectIn
	);
	virtual ~OrientedBoundingBox() {};

protected:
	Debugable* DebugObject = nullptr;

protected:
	SPosition3D			Center;
	SVector3D			Extents;
	DirectX::XMMATRIX	Orientation;


public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) override;
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void Render(PSOObject* PSOObjectIn) override;
};

