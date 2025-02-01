#pragma once
#include "IComponentVisitor.h"
#include <d3d11.h>

class AViewComponent;

class ComponentEntityInitializer : public IComponentVisitor
{
public:
	ComponentEntityInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~ComponentEntityInitializer() override = default;

protected:
	ID3D11Device* m_deviceCached = nullptr;
	ID3D11DeviceContext* m_deviceContextCached = nullptr;

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
	void InitBaseComponent(AComponent* component);
	void InitViewComponent(AViewComponent* viewComponent);
};

