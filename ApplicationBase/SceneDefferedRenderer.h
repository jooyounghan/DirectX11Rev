#pragma once
#include "ASceneRenderer.h"

class SceneDefferedRenderer : public ASceneRenderer
{
public:
	SceneDefferedRenderer(
		ID3D11DeviceContext** deviceContextAddress, 
		ComponentPSOManager* componentPsoManager
	);
	~SceneDefferedRenderer() override = default;

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;
};

