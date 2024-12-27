#pragma once
#include "mysqlx/xdevapi.h"
#include "DBErrorHandler.h"
#include "AssetManager.h"
#include "IComponentVisitor.h"

class ComponentInitializer : public IComponentVisitor, public DBErrorHandler
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
	virtual void Visit(StaticModelComponent* staticModelComponent) override;
	virtual void Visit(SkeletalModelComponent* skeletalModelComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;


};

