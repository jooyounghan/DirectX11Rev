#pragma once
#include "ASceneRenderer.h"

class ModelMaterialAsset;

class SceneForwardRenderer : public ASceneRenderer
{
public:
	SceneForwardRenderer(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress,
		Scene* const* sceneAddress
	);
	~SceneForwardRenderer() override = default;

protected:
	std::vector<ModelMaterialAsset*> m_selectedModelMaterialAssets;

public:
	virtual void Visit(StaticMeshComponent* staticMeshComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;


protected:
	void ApplyMainFilmWithIDChannel(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent);
	void RenderMeshPartHandler(const size_t& idx);
};

