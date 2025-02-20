#pragma once
#include "IComponentVisitor.h"

#include <mysqlx/xdevapi.h>

struct ID3D11Device;
class AViewComponent;
class ICollisionOption;
class LightComponent;

class ComponentDBInitializer : public IComponentVisitor
{
public:
	ComponentDBInitializer(mysqlx::Schema* schema);
	~ComponentDBInitializer() override = default;

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
	void LoadStaticMeshComponent(StaticMeshComponent* staticMeshComponent);
	void LoadSkeletalMeshComponent(SkeletalMeshComponent* skeletalMeshComponent);
	void LoadModelMaterials(AMeshComponent* meshComponent);

private:
	void LoadCameraComponent(CameraComponent* cameraComponent);

private:
	void LoadSphereColiisionComponent(SphereCollisionComponent* sphereCollisionComponent);
	void LoadOrientedBoxCollisionComponent(OrientedBoxCollisionComponent* orientedBoxCollisionComponent);
	ICollisionOption* CreateCollisionOption(const uint32_t& sceneID, const uint32_t& collitionOptionID);
	void LoadLightComponent(LightComponent* lightComponent);
	void LoadSpotLightComponent(SpotLightComponent* spotLightComponent);
};

