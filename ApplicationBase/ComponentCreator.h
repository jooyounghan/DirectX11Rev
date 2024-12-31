#pragma once
#include "ComponentType.h"
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

class ComponentCreator : public IComponentVisitor
{
public:
	ComponentCreator(mysqlx::Schema* schema);
	~ComponentCreator() override = default;

protected:
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	void AddScene(Scene* scene);
	void AddComponent(Scene* scene, AComponent* parentComponent, AComponent* component);

private:
	void AddMeshComponent(AMeshComponent* meshComponent);
	void AddComponetToType(AComponent* component, const EComponentType& componentType);
};

