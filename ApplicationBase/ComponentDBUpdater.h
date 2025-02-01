#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

struct ID3D11DeviceContext;
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
	void UpdateLightEntity(AComponent* component, LightEntity* ligthEntity);
};

