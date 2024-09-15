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
	virtual void Visit(StaticMeshObjectActor* MeshObjectActorInstance) override;
	virtual void Visit(SkeletalMeshObjectActor* MeshObjectActorInstance) override;
	virtual void Visit(EnvironmentActor* EnvironmentActorInstance) override;
	virtual void Visit(EditorPawn* ActorInstance) override;

public:
	virtual void Visit(StaticMeshObject* StaticMeshObjectInstance);
	virtual void Visit(SkeletalMeshObject* SkeletalMeshObjectInstance);
	virtual void Visit(BoundingSphereObject* BoundingSphereInstance) override;
	virtual void Visit(OBBObject* OBBInstance) override;
	virtual void Visit(BoundingFrustumObject* BoundingFrustumInstance) override;
	virtual void Visit(Viewable* BoundingFrustumInstance) override;
};



