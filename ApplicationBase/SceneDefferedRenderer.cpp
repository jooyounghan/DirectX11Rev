#include "SceneDefferedRenderer.h"

SceneDefferedRenderer::SceneDefferedRenderer(
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress,
	Scene* const* sceneAddress
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager, cameraComponentAddress, sceneAddress)
{
}

void SceneDefferedRenderer::Visit(StaticMeshComponent* staticMeshComponent)
{
}

void SceneDefferedRenderer::Visit(SkeletalMeshComponent* skeletalMeshComponent)
{
}

void SceneDefferedRenderer::Visit(CameraComponent* cameraComponent)
{
}
