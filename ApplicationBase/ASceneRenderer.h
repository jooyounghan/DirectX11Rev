#pragma once
#include "IComponentVisitor.h"
#include "ISceneVisitor.h"

#include <stdint.h>
#include <functional>
#include <string>

struct ID3D11DeviceContext;
class ComponentPSOManager;
class GraphicsPSOObject;

class AMeshPartsData;
class ModelMaterialAsset;
class AComponent;
class ACollisionComponent;
class ScreenQuad;

constexpr float ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };

class ASceneRenderer : public IComponentVisitor, public ISceneVisitor
{
public:
	ASceneRenderer(
		ID3D11DeviceContext* deviceContex,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress,
		Scene* const* sceneAddress
	);
	~ASceneRenderer() override = default;

protected:
	ID3D11DeviceContext* m_deviceContext = nullptr;
	ComponentPSOManager* m_componentPsoManagerCached = nullptr;
	CameraComponent* const* m_selectedCameraComponentAddressCached = nullptr;
	Scene* const* m_sceneAddressCached = nullptr;

protected:
	std::vector<ModelMaterialAsset*> m_selectedModelMaterialAssets;
	ScreenQuad* m_screenQuad = nullptr;

public:
	virtual void Visit(Scene* scene) override;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) = 0;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

public:
	virtual void Visit(SpotLightComponent* spotLightComponent) override;
	virtual void Visit(PointLightComponent* pointLightComponent) override;

public:
	virtual void ClearRenderTargets();
	virtual void PostProcess() = 0;

protected:
	uint32_t GetLODLevel(const size_t& maxLODLevel, const AComponent* component) const;
	void RenderMeshParts(
		ID3D11DeviceContext* const deviceContext,
		const AMeshPartsData* const meshPartsData,
		std::function<void(const size_t&)> handler = [&](const size_t&) {}
	);

protected:
	virtual void ApplyRenderTargets(
		ID3D11DeviceContext* const deviceContext, 
		CameraComponent* const cameraComponent
	) const ;
	virtual void ApplyRenderTargetsWithID(
		ID3D11DeviceContext* const deviceContext,
		CameraComponent* const cameraComponent
	) const;

	virtual void RenderMeshPartHandler(const size_t& idx) = 0;

private:
	void RenderCollisionComponent(ACollisionComponent* collisionComponent, const std::string& debugStaticMeshAssetName);
};

