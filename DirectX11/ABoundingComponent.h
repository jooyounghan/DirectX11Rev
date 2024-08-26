#pragma once
#include "IIntersectable.h"
#include "AttachableObject.h"

class GraphicsPipeline;
class Debugable;

class ABoundingComponent : public IIntersectable, public AttachableObject
{	
public:
	ABoundingComponent(GraphicsPipeline* GraphicsPipelineInstances);
	virtual ~ABoundingComponent();

protected:
	Debugable* DebugObject = nullptr;
	MakeGetter(DebugObject);

protected:
	bool IsCollided = false;

protected:
	UploadBuffer<XMVECTOR> DebuggingColorBuffer;
	MakeGetter(DebuggingColorBuffer);

public:
	void UpdateColor(const XMVECTOR& ColorIn, ID3D11DeviceContext* DeviceContextIn);

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) = 0; 

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) = 0;
	void SetCollisionColor();

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override = 0;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void OnSerialize(FILE* FileIn) = 0;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

