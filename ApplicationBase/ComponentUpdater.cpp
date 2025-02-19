#include "ComponentUpdater.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "DynamicBuffer.h"

ComponentUpdater::ComponentUpdater(ID3D11DeviceContext* deviceContext, const float& deltaTime)
	: m_deviceContextCached(deviceContext), m_deltaTime(deltaTime)
{
}

void ComponentUpdater::Visit(StaticMeshComponent* staticModelComponent)
{
	UpdateBaseComponent(staticModelComponent);
}

void ComponentUpdater::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	UpdateBaseComponent(skeletalModelComponent);

	AnimationPlayer* animationPlayer = skeletalModelComponent->GetAnimationPlayer();
	if (animationPlayer) animationPlayer->UpdateAnimationPlayer(m_deviceContextCached, m_deltaTime);
}

void ComponentUpdater::Visit(CameraComponent* cameraComponent)
{
	UpdateBaseComponent(cameraComponent);
	UpdateViewComponent(cameraComponent);
}

void ComponentUpdater::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	UpdateBaseComponent(sphereCollisionComponent);
	sphereCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentUpdater::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	UpdateBaseComponent(orientedBoxCollisionComponent);
	orientedBoxCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentUpdater::Visit(SpotLightComponent* spotLightComponent)
{
	UpdateBaseComponent(spotLightComponent);
	UpdateViewComponent(spotLightComponent);
	UpdateLightComponent(spotLightComponent);
}

void ComponentUpdater::Visit(PointLightComponent* pointLightComponent)
{
	UpdateBaseComponent(pointLightComponent);
	pointLightComponent->UpdatePointLightParts();
	UpdateLightComponent(pointLightComponent);

	for (size_t idx = 0; idx < 6; ++idx)
	{
		PointLightFrustumPart* pointLightFrustumPart = pointLightComponent->GetPointLightFrustumPart(idx);

		DynamicBuffer* viewProjMatrixBuffer = pointLightFrustumPart->GetViewProjMatrixBuffer();
		viewProjMatrixBuffer->Upload(m_deviceContextCached);
	}
}

void ComponentUpdater::UpdateBaseComponent(AComponent* component)
{
	DynamicBuffer& componentEntityBuffer = component->GetComponentEntityBuffer();
	componentEntityBuffer.Upload(m_deviceContextCached);

	component->UpdateEntity();
	DynamicBuffer& transformationEntityBuffer = component->GetTransformationEntityBuffer();
	transformationEntityBuffer.Upload(m_deviceContextCached);
}

void ComponentUpdater::UpdateViewComponent(AViewComponent* viewComponent)
{
	viewComponent->UpdateViewEntity();
	DynamicBuffer* viewProjMatrixBuffer = viewComponent->GetViewProjMatrixBuffer();
	viewProjMatrixBuffer->Upload(m_deviceContextCached);
}

void ComponentUpdater::UpdateLightComponent(LightEntity* lightComponent)
{
	DynamicBuffer* lightBuffer = lightComponent->GetLightEntityBuffer();
	lightBuffer->Upload(m_deviceContextCached);

}

