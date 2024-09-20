#pragma once
#include "IIntersectable.h"
#include "AAttachableObject.h"
#include <DirectXCollision.h>

class GraphicsPipeline;
class Debugable;

class ABoundingObject : public IIntersectable, public AAttachableObject
{	
public:
	ABoundingObject();
	virtual ~ABoundingObject();

protected:
	Debugable* DebugObject = nullptr;
	MakeGetter(DebugObject);

protected:
	bool IsCollided = false;

protected:
	UploadBuffer<DirectX::XMVECTOR> DebuggingColorBuffer;
	MakeGetter(DebuggingColorBuffer);

public:
	void UpdateColor(const DirectX::XMVECTOR& ColorIn, ID3D11DeviceContext* DeviceContextIn);

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) = 0;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) = 0;
	void SetCollisionColor();

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;
};

