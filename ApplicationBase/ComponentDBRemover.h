#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

class ComponentDBRemover : public IComponentVisitor
{
public:
	ComponentDBRemover(mysqlx::Schema* schema);
	~ComponentDBRemover() override = default;

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

