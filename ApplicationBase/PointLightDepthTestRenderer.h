#pragma once
#include "ADepthTestRenderer.h"

struct ID3D11Buffer;
struct D3D11_VIEWPORT;
struct ID3D11DepthStencilView;

class PointLightDepthTestRenderer : public ADepthTestRenderer
{
public:
	PointLightDepthTestRenderer(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager,
		ID3D11Buffer* viewEntityBufferCached,
		const D3D11_VIEWPORT* viewportCached,
		ID3D11DepthStencilView* depthStencilViewCached
	);
	~PointLightDepthTestRenderer() override = default;

protected:
	ID3D11Buffer* m_viewEntityBufferCached = nullptr;
	const D3D11_VIEWPORT* m_viewportCached;
	ID3D11DepthStencilView* m_depthStencilViewCached = nullptr;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

public:
	virtual void Visit(SpotLightComponent* spotLightComponent) override;
	virtual void Visit(PointLightComponent* pointLightComponent) override;
};

