#pragma once
#include "IIntersectable.h"
#include "RelativePlaceableObject.h"

class GraphicsPipeline;
class AssetManager;
class Debugable;

class ABoundingComponent : public IIntersectable, public RelativePlaceableObject
{
public:
	ABoundingComponent(GraphicsPipeline* GraphicsPipelineInstances, AssetManager* AssetManagerInstance);
	virtual ~ABoundingComponent();

protected:
	Debugable* DebugObject = nullptr;
	bool IsCollided = false;

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) = 0; 

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) = 0;
	void SetCollisionColor();

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Render(PSOObject* PSOObjectIn) override final;

public:
	virtual void AcceptGui(IGuiLowLevelVisitor* GuiVisitor) = 0;
};

