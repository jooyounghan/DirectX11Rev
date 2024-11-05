#pragma once
#include "ABoundingObject.h"
#include "GlobalVariable.h"

class BoundingSphereObject;

class OBBObject : public ABoundingObject, public DirectX::BoundingOrientedBox
{
public:
	OBBObject(MapAsset* MapAssetInstance);

public:
	OBBObject(
		MapAsset* MapAssetInstance,
		const float& HalfXIn, 
		const float& HalfYIn, 
		const float& HalfZIn
	);
	virtual ~OBBObject();

public:
	static std::string BoundingOBBKind;

private:
	void InitOBB(ID3D11Device* DeviceIn);


public:
	DirectX::XMFLOAT3 DescaledExtents;

protected:
	static std::shared_ptr<Debugable> CreateDebugBoxObject(ID3D11Device* DeviceIn);

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
