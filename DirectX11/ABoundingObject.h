#pragma once
#include "IIntersectable.h"
#include "AAttachableObject.h"
#include "directxmath/DirectXCollision.h"

class GraphicsPipeline;
class Debugable;
class PSOObject;

class ABoundingObject : public IIntersectable, public AAttachableObject
{	
public:
	ABoundingObject(const std::string& AttachableKindIn);
	virtual ~ABoundingObject();

protected:
	Debugable* DebugObject = nullptr;
	MakeGetter(DebugObject);

protected:
	bool IsCollided = false;

protected:
	std::shared_ptr<UploadBuffer<DirectX::XMVECTOR>> DebuggingColorBuffer;
	MakeGetter(DebuggingColorBuffer);

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) = 0;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) = 0;
	void SetCollisionColor();

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

protected:
	PSOObject* BoundingObjectPSOCached = nullptr;

public:
	virtual void Render(MapAsset* MapAssetIn) override final;
};

