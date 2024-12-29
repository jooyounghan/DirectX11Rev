#include "SceneDefferedRenderer.h"

SceneDefferedRenderer::SceneDefferedRenderer(
	ID3D11DeviceContext** deviceContextAddress, 
	PSOManager* psoManager
)
	: ASceneRenderer(deviceContextAddress, psoManager)
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
