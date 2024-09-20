#pragma once
#include "ABoundingObject.h"
#include "DefineType.h"
#include "GlobalVariable.h"

class Viewable;
class BoundingSphereObject;
class OBBObject;

class BoundingFrustumObject : public ABoundingObject, public BoundingFrustum
{
public:
	BoundingFrustumObject(MapAsset* MapAssetInstance, Viewable* ViewableInstance);
	virtual ~BoundingFrustumObject();

protected:
	Viewable* ViewableCached = nullptr;

public:
	static const char* BoundingFrustumIdentifier;

private:
	static size_t BoundingFrustumCount;
	static std::shared_ptr<Debugable> CreateDebugFrustumObject(ID3D11Device* DeviceIn);

public:
	virtual bool Intersect(Ray* RayIn, float& DistanceOut) override;

public:
	virtual bool AcceptCollision(ICollisionVisitor* CollisionVisitor) override;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

