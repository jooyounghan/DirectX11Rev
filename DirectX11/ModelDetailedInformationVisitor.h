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
	virtual void Visit(Actor* ActorInstance) override;
	virtual void Visit(EditorActor* ActorInstance) override;

public:
	virtual void Visit(MeshObject* AttachableInstance) override;
	virtual void Visit(BoundingSphereObject* BoundingSphereInstance) override;
	virtual void Visit(OBBObject* OBBInstance) override;
	virtual void Visit(BoundingFrustumObject* BoundingFrustumInstance) override;
	virtual void Visit(Viewable* BoundingFrustumInstance) override;
};



