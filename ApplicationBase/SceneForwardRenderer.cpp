#include "SceneForwardRenderer.h"

SceneForwardRenderer::SceneForwardRenderer(
	ID3D11DeviceContext** deviceContextAddress,
	PSOManager* psoManager
)
	: ASceneRenderer(deviceContextAddress, psoManager)
{
}

void SceneForwardRenderer::Visit(StaticMeshComponent* staticMeshAsset)
{

}

void SceneForwardRenderer::Visit(SkeletalMeshComponent* skeletalMeshAsset)
{
}

void SceneForwardRenderer::Visit(CameraComponent* cameraComponent)
{
}
