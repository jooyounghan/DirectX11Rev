#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

class ComponentUpdater : public IComponentVisitor
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

