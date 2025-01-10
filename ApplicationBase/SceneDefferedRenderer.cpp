#include "SceneDefferedRenderer.h"

SceneDefferedRenderer::SceneDefferedRenderer(
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager, cameraComponentAddress)
{
}

void SceneDefferedRenderer::Visit(StaticMeshComponent* staticMeshAsset)
{
}

void SceneDefferedRenderer::Visit(SkeletalMeshComponent* skeletalMeshAsset)
{
}

void SceneDefferedRenderer::Visit(CameraComponent* cameraComponent)
{
}
