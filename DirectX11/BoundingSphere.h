#pragma once
#include "ABoundingComponent.h"
 
class BoundingSphere : public ABoundingComponent
{
public:
	BoundingSphere(
		GraphicsPipeline* GraphicsPipelineInstances,
		AssetManager* AssetManagerInstance,
		const float& RadiusIn
	);
	virtual ~BoundingSphere();

public:
	virtual bool Intersect(const Ray& RayIn, float& DistanceOut) override;
};

