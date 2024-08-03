#pragma once
#include "ABoundingComponent.h"

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

public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) override;
};

