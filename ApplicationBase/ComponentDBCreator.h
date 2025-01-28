#pragma once
#include "ComponentType.h"
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

class Scene;

class ComponentDBCreator : public IComponentVisitor
{
public:
	ComponentDBCreator(mysqlx::Schema* schema);
	~ComponentDBCreator() override = default;

protected:
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(ARenderSphereCollisionComponent* renderSphereCollisionComponent) override;
	virtual void Visit(ARenderOrientedBoxCollisionComponent* renderOrientedBoxCollisionComponent) override;
	virtual void Visit(ARenderFrustumCollisionComponent* renderFrustumCollisionComponent) override;

public:
	void AddScene(Scene* scene);
	void AddComponent(Scene* scene, AComponent* parentComponent, AComponent* component);

private:
	void AddMeshComponent(AMeshComponent* meshComponent);
	void AddComponetToType(AComponent* component, const EComponentType& componentType);
};

