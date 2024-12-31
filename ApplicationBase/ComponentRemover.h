#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

class ComponentRemover : public IComponentVisitor
{
public:
	ComponentRemover(mysqlx::Schema* schema);
	~ComponentRemover() override = default;

protected:
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void DeleteComponent(AComponent* component);
	void DeleteComponetFromSceneInformation(AComponent* component);
	void DeleteMeshComponent(AMeshComponent* meshComponent);
};

