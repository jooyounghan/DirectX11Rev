#pragma once
#include "IComponentVisitor.h"
#include "ISceneVisitor.h"
#include <stdint.h>
#include <functional>

struct ID3D11DeviceContext;
class AComponent;
class ComponentPSOManager;

class MeshPartsData;

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

public:
	virtual void Visit(Scene* scene) override final;

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) = 0;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) = 0;

public:
	virtual void Visit(CameraComponent* cameraComponent) = 0;

public:
	virtual void ClearRenderTargets();

protected:
	uint32_t GetLODLevel(const AComponent* component) const;
	void ApplyMainFilmCamera(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent) const;
	void RenderMeshParts(
		ID3D11DeviceContext* const deviceContext, 
		const MeshPartsData* const meshPartsData,
		std::function<void(const size_t&)> handler = [&](const size_t&) {}
	);
};

