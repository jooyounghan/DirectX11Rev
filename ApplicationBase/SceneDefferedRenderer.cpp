#include "SceneDefferedRenderer.h"

SceneDefferedRenderer::SceneDefferedRenderer(
	ID3D11DeviceContext** deviceContextAddress, 
	ComponentPSOManager* componentPsoManager
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager)
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
