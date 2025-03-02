#pragma once
#include "ADepthTestRenderer.h"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct D3D11_VIEWPORT;
struct ID3D11DepthStencilView;

class SpotLightDepthTestRenderer : public ADepthTestRenderer
{
public:
	SpotLightDepthTestRenderer(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager,
		ID3D11Buffer* lightComponentEntityBuffer,
		ID3D11ShaderResourceView* const viewEntityStructuredBuffer,
		const D3D11_VIEWPORT* viewport,
		ID3D11DepthStencilView* depthStencilView
	);
	~SpotLightDepthTestRenderer() override = default;

protected:
	ID3D11Buffer* m_lightComponentEntityBuffer = nullptr;
	ID3D11ShaderResourceView* const m_viewEntityStructuredBuffer = nullptr;
	const D3D11_VIEWPORT* m_viewport = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;

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