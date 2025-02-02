#include "DepthTestRenderer.h"

DepthTestRenderer::DepthTestRenderer(
	ID3D11DeviceContext* const* deviceContextAddress, 
	ComponentPSOManager* componentPsoManager, 
	CameraComponent* const* cameraComponentAddress, 
	Scene* const* sceneAddress
)
	: ASceneRenderer(deviceContextAddress, componentPsoManager, cameraComponentAddress, sceneAddress)
{
}

void DepthTestRenderer::Visit(Scene* scene)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(StaticMeshComponent* staticMeshAsset)
{

}

void DepthTestRenderer::Visit(SkeletalMeshComponent* skeletalMeshAsset)
{

}

void DepthTestRenderer::Visit(CameraComponent* cameraComponent)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(SpotLightComponent* spotLightComponent)
{
	// Do Nothing
}

void DepthTestRenderer::Visit(PointLightComponent* pointLightComponent)
{
	// Do Nothing
}
