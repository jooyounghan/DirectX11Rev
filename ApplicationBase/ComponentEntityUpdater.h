#pragma once
#include "IComponentVisitor.h"
#include <d3d11.h>

class AViewComponent;
class LightEntity;

class ComponentEntityUpdater : public IComponentVisitor
{
public:
	ComponentEntityUpdater(ID3D11DeviceContext* deviceContext, const float& deltaTime);
	~ComponentEntityUpdater() override = default;

protected:
	ID3D11DeviceContext* m_deviceContextCached = nullptr;
	const float& m_deltaTime;

public:
	virtual void Visit(StaticMeshComponent* staticModelComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalModelComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

public:
	virtual void Visit(SpotLightComponent* spotLightComponent) override final;
	virtual void Visit(PointLightComponent* pointLightComponent) override final;

private:
	void UpdateBaseComponent(AComponent* component);
	void UpdateViewComponent(AViewComponent* viewComponent);
	void UpdateLightComponent(LightEntity* lightComponent);
};