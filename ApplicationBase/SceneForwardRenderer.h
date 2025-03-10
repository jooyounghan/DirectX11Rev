#pragma once
#include "ASceneRenderer.h"

class SceneForwardRenderer : public ASceneRenderer
{
public:
	SceneForwardRenderer(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress,
		Scene* const* sceneAddress
	);
	~SceneForwardRenderer() override = default;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void PostProcess() override;

protected:
	virtual void RenderMeshPartHandler(const size_t& idx) override;
};

