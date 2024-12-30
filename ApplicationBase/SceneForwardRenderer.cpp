#include "SceneForwardRenderer.h"

SceneForwardRenderer::SceneForwardRenderer(
	ID3D11DeviceContext** deviceContextAddress,
	ComponentPSOManager* componentPsoManager
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager)
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
