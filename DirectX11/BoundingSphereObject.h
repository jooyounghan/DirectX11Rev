#pragma once
#include "ABoundingObject.h"
 
class OBBObject;

class BoundingSphereObject : public ABoundingObject, public DirectX::BoundingSphere
{
public:
	BoundingSphereObject(MapAsset* MapAssetInstance);
	BoundingSphereObject(MapAsset* MapAssetInstance, const float& RadiusIn);
	virtual ~BoundingSphereObject();

public:
	static std::string BoundingSphereKind;

private:
	void InitBoundingSphere(ID3D11Device* DeviceIn);

private:
	static std::shared_ptr<Debugable> CreateDebugSphereObject(ID3D11Device* DeviceIn);

protected:
	float DescaledRadius;
	MakeGetter(DescaledRadius);
	MakePointerGetter(DescaledRadius);

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

