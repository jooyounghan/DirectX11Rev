#pragma once
#include "IComponentVisitor.h"
#include "ISceneVisitor.h"
#include <d3d11.h>

class LightComponent;

class ComponentUpdater : public IComponentVisitor, public ISceneVisitor
{
public:
	ComponentUpdater(ID3D11DeviceContext* deviceContext, const float& deltaTime);
	~ComponentUpdater() override = default;

protected:
	ID3D11DeviceContext* m_deviceContextCached = nullptr;
	const float& m_deltaTime;

public:
	virtual void Visit(Scene* scene) override;

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
	bool UpdateBaseComponent(AComponent* component);
};