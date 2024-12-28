#pragma once
#include "mysqlx/xdevapi.h"
#include "AssetManager.h"
#include "IComponentVisitor.h"


class ComponentInitializer : public IComponentVisitor
{
public:
	ComponentInitializer(
		AssetManager* assetManager,
		ID3D11Device** deviceAddress,
		mysqlx::Schema* schema
	);

protected:
	AssetManager* m_assetManagerCached = nullptr;
	ID3D11Device** m_deviceAdressCached = nullptr;
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void LoadModelMaterials(AMeshComponent* meshComponent);
};

