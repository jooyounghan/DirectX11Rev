#pragma once
#include "IGuiModelVisitor.h"
#include <DirectXMath.h>

class ModelDetailWindow;
class AssetManager;

class ModelDetailedInformationVisitor : public IGuiModelVisitor
{
public:
	ModelDetailedInformationVisitor(AssetManager* AssetManagerInstanceIn);

protected:
	AssetManager* AssetManagerCached;

public:
	virtual void Visit(PlaceableObject* PlaceableInstance) override;

public:
	virtual void Visit(MeshObject* AttachableInstance) override;

public:
	virtual void Visit(BoundingSphereObject* BoundingSphereInstance) override;
	virtual void Visit(OBBObject* OBBInstance) override;
	virtual void Visit(BoundingFrustumObject* BoundingFrustumInstance) override;

public:
	virtual void Visit(Viewable* BoundingFrustumInstance) override;

};



