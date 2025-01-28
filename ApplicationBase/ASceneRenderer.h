#pragma once
#include "IComponentVisitor.h"
#include "ISceneVisitor.h"

#include <stdint.h>
#include <functional>

struct ID3D11DeviceContext;
class AComponent;
class ComponentPSOManager;
class ModelMaterialAsset;
class MeshPartsData;
class GraphicsPSOObject;
class ScreenQuad;

constexpr float ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };

class ASceneRenderer : public IComponentVisitor, public ISceneVisitor
{
public:
	ASceneRenderer(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress,
		Scene* const* sceneAddress
	);
	~ASceneRenderer() override = default;

protected:
	ID3D11DeviceContext* const* m_deviceContextAddress = nullptr;
	ComponentPSOManager* m_componentPsoManagerCached = nullptr;
	CameraComponent* const* m_selectedCameraComponentAddressCached = nullptr;
	Scene* const* m_sceneAddressCached = nullptr;

protected:
	std::vector<ModelMaterialAsset*> m_selectedModelMaterialAssets;
	ScreenQuad* m_screenQuad;

public:
	virtual void Visit(Scene* scene) override final;

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) = 0;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;

public:
	virtual void Visit(ARenderSphereCollisionComponent* renderSphereCollisionComponent) override;
	virtual void Visit(ARenderOrientedBoxCollisionComponent* renderOrientedBoxCollisionComponent) override;
	virtual void Visit(ARenderFrustumCollisionComponent* renderFrustumCollisionComponent) override;

public:
	virtual void ClearRenderTargets();
	virtual void PostProcess() = 0;

protected:
	uint32_t GetLODLevel(const AComponent* component) const;
	void RenderMeshParts(
		ID3D11DeviceContext* const deviceContext,
		const MeshPartsData* const meshPartsData,
		std::function<void(const size_t&)> handler = [&](const size_t&) {}
	);

protected:
	virtual void ApplyRenderTargets(
		ID3D11DeviceContext* const deviceContext, 
		const CameraComponent* const cameraComponent
	) const = 0;

	virtual void RenderMeshPartHandler(const size_t& idx) = 0;
};

