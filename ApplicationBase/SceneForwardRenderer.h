#pragma once
#include "ASceneRenderer.h"

class SceneForwardRenderer : public ASceneRenderer
{
public:
	SceneForwardRenderer(
		ID3D11DeviceContext** deviceContextAddress, 
		ComponentPSOManager* componentPsoManager
	);

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;
};

