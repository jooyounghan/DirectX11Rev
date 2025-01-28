#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

class AssetManager;
struct ID3D11Device;

class ComponentDBInitializer : public IComponentVisitor
{
public:
	ComponentDBInitializer(
		AssetManager* assetManager,
		mysqlx::Schema* schema
	);
	~ComponentDBInitializer() override = default;

protected:
	AssetManager* m_assetManagerCached = nullptr;
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(ARenderSphereCollisionComponent* renderSphereCollisionComponent) override;
	virtual void Visit(ARenderOrientedBoxCollisionComponent* renderOrientedBoxCollisionComponent) override;
	virtual void Visit(ARenderFrustumCollisionComponent* renderFrustumCollisionComponent) override;

private:
	void LoadModelMaterials(AMeshComponent* meshComponent);
};

