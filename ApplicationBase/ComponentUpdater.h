#pragma once
#include "mysqlx/xdevapi.h"
#include "DBErrorHandler.h"
#include "IComponentVisitor.h"

class Scene;

class ComponentUpdater : public IComponentVisitor, public DBErrorHandler
{
public:
	ComponentUpdater(mysqlx::Schema* schema);

protected:
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void UpdateComponent(AComponent* component);
	void UpdateMeshComponent(AMeshComponent* meshComponent);
};

