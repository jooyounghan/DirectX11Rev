#include "ComponentEntityUpdater.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "DynamicBuffer.h"

ComponentEntityUpdater::ComponentEntityUpdater(ID3D11DeviceContext* deviceContext, const float& deltaTime)
	: m_deviceContextCached(deviceContext), m_deltaTime(deltaTime)
{
}

void ComponentEntityUpdater::Visit(StaticMeshComponent* staticModelComponent)
{
	UpdateBaseComponent(staticModelComponent);
}

void ComponentEntityUpdater::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	UpdateBaseComponent(skeletalModelComponent);

	AnimationPlayer* animationPlayer = skeletalModelComponent->GetAnimationPlayer();
	if (animationPlayer) animationPlayer->UpdateAnimationPlayer(m_deviceContextCached, m_deltaTime);
}

void ComponentEntityUpdater::Visit(CameraComponent* cameraComponent)
{
	UpdateBaseComponent(cameraComponent);
	UpdateViewComponent(cameraComponent);
}

void ComponentEntityUpdater::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	UpdateBaseComponent(sphereCollisionComponent);
	sphereCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentEntityUpdater::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	UpdateBaseComponent(orientedBoxCollisionComponent);
	orientedBoxCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentEntityUpdater::Visit(SpotLightComponent* spotLightComponent)
{
	UpdateBaseComponent(spotLightComponent);
	UpdateViewComponent(spotLightComponent);
	UpdateLightComponent(spotLightComponent);
}

void ComponentEntityUpdater::Visit(PointLightComponent* pointLightComponent)
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

void ComponentEntityUpdater::UpdateBaseComponent(AComponent* component)
{
	DynamicBuffer* componentBuffer = component->GetComponentBuffer();
	componentBuffer->Upload(m_deviceContextCached);

	component->UpdateAbsoluteEntities();
	component->UpdateComponentTransformation();
	DynamicBuffer* transformationBuffer = component->GetTransformationBuffer();
	transformationBuffer->Upload(m_deviceContextCached);
}

void ComponentEntityUpdater::UpdateViewComponent(AViewComponent* viewComponent)
{
	viewComponent->UpdateViewEntity();
	DynamicBuffer* viewProjMatrixBuffer = viewComponent->GetViewProjMatrixBuffer();
	viewProjMatrixBuffer->Upload(m_deviceContextCached);
}

void ComponentEntityUpdater::UpdateLightComponent(LightEntity* lightComponent)
{
	DynamicBuffer* lightBuffer = lightComponent->GetLightEntityBuffer();
	lightBuffer->Upload(m_deviceContextCached);

}

