#pragma once
#include "ABoundingObject.h"
#include "GlobalVariable.h"

class Viewable;
class BoundingSphereObject;
class OBBObject;

/*
TODO : Viewable* ViewableInstance를 인자로 받아야만 할까?
*/

class BoundingFrustumObject : public ABoundingObject, public DirectX::BoundingFrustum
{
public:
	BoundingFrustumObject(MapAsset* MapAssetInstance, Viewable* ViewableInstance);
	virtual ~BoundingFrustumObject();

public:
	static std::string BoundingFrustumKind;

protected:
	Viewable* ViewableCached = nullptr;

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
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

