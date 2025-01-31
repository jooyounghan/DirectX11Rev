#pragma once
#include "IComponentVisitor.h"
#include <d3d11.h>

class ComponentEntityUpdater : public IComponentVisitor
{
public:
	ComponentEntityUpdater(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const float& deltaTime);
	virtual ~ComponentEntityUpdater() override = default;

protected:
	ID3D11Device* m_deviceCached = nullptr;
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

private:
	void UpdateComponentBuffer(AComponent* component);
	void UpdateTransformationBuffer(AComponent* component);
};