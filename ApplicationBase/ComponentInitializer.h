#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

class AssetManager;
struct ID3D11Device;

class ComponentInitializer : public IComponentVisitor
{
public:
	ComponentInitializer(
		AssetManager* assetManager,
		ID3D11Device* const* deviceAddress,
		mysqlx::Schema* schema
	);
	~ComponentInitializer() override = default;

protected:
	AssetManager* m_assetManagerCached = nullptr;
	ID3D11Device* const* m_deviceAdressCached = nullptr;
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void LoadModelMaterials(AMeshComponent* meshComponent);
};

