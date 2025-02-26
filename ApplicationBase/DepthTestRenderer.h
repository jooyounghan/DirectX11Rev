#pragma once
#include "IComponentVisitor.h"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct D3D11_VIEWPORT;
class ComponentPSOManager;
class AMeshPartsData;

class Scene;

class DepthTestRenderer : public IComponentVisitor
{
public:
	DepthTestRenderer(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager,
		ID3D11Buffer* lightComponentEntityBuffer,
		ID3D11ShaderResourceView* const viewEntityStructuredBuffer,
		const D3D11_VIEWPORT* viewport,
		ID3D11DepthStencilView* depthStencilView
	);
	~DepthTestRenderer() override = default;

protected:
	ID3D11DeviceContext* m_deviceContext = nullptr;
	ComponentPSOManager* m_componentPsoManagerCached = nullptr;
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

protected:
	void DepthTestMeshParts(const AMeshPartsData* const meshPartsData);
};