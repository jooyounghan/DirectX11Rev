#pragma once
#include "mysqlx/xdevapi.h"
#include "IComponentVisitor.h"

class ComponentCreator : public IComponentVisitor
{
public:
	ComponentCreator(mysqlx::Schema* schema);

protected:
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

};

