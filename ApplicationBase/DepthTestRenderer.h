#pragma once
#include "IComponentVisitor.h"

struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct D3D11_VIEWPORT;
class ComponentPSOManager;
class AMeshPartsData;

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class DSVOption;

class Scene;

class DepthTestRenderer : public IComponentVisitor
{
public:
	DepthTestRenderer(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		ID3D11Buffer* viewProjMatrix,
		D3D11_VIEWPORT* viewport,
		Texture2DInstance<SRVOption, DSVOption>* depthStencilViewTexture
	);
	~DepthTestRenderer() override = default;

protected:
	ID3D11DeviceContext* const* m_deviceContextAddress = nullptr;
	ComponentPSOManager* m_componentPsoManagerCached = nullptr;
	ID3D11Buffer* m_viewProjMatrix = nullptr;
	D3D11_VIEWPORT* m_viewport = nullptr;
	Texture2DInstance<SRVOption, DSVOption>* m_depthStencilViewTexture = nullptr;

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
	void DepthTestMeshParts(
		ID3D11DeviceContext* const deviceContext,
		const AMeshPartsData* const meshPartsData
	);
};