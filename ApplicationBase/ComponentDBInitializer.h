#pragma once
#include "DBErrorHandler.h"
#include "AssetManager.h"
#include "IComponentVisitor.h"
#include "ComponentManager.h"

class ComponentDBInitializer : public IComponentVisitor, public DBErrorHandler
{
public:
	ComponentDBInitializer(
		ID3D11Device* device,
		AssetManager* assetManager,
		mysqlx::Schema* schema
	);

protected:
	ID3D11Device* m_deviceCached = nullptr;
	AssetManager* m_assetManagerCached = nullptr;
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticModelComponent* staticModelComponent) override;
	virtual void Visit(SkeletalModelComponent* skeletalModelComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;


};

