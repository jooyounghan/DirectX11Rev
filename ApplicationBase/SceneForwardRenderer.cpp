#include "SceneForwardRenderer.h"

SceneForwardRenderer::SceneForwardRenderer(
	ID3D11DeviceContext* const* deviceContextAddress,
	ComponentPSOManager* componentPsoManager,
	CameraComponent* const* cameraComponentAddress
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager, cameraComponentAddress)
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
