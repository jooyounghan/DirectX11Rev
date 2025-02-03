#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

struct ID3D11DeviceContext;
class AViewComponent;
class LightEntity;

class ComponentDBUpdater : public IComponentVisitor
{
public:
	ComponentDBUpdater(mysqlx::Schema* schema);
	~ComponentDBUpdater() override = default;

protected:
	mysqlx::Schema* m_schemaCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

public:
	virtual void Visit(SpotLightComponent* spotLightComponent) override final;
	virtual void Visit(PointLightComponent* pointLightComponent) override final;

private:
	void UpdateComponent(AComponent* component);
	void UpdateMeshComponent(AMeshComponent* meshComponent);

private:
	void UpdateStaticMeshComponent(StaticMeshComponent* staticMeshComponent);
	void UpdateSkeletalMeshComponent(SkeletalMeshComponent* skeletalMeshComponent);

private:
	void UpdateViewComponent(AViewComponent* viewComponent);
	void UpdateCameraComponent(CameraComponent* cameraComponent);

private:
	void UpdateSphereCollisionComponent(SphereCollisionComponent* sphereCollisionComponent);
	void UpdateOrientedBoxCollisionComponent(OrientedBoxCollisionComponent* orientedBoxCollisionComponent);

private:
	void UpdateLightEntity(AComponent* component, LightEntity* ligthEntity);
};

