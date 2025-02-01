#include "ComponentEntityUpdater.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "DynamicBuffer.h"

ComponentEntityUpdater::ComponentEntityUpdater(ID3D11DeviceContext* deviceContext, const float& deltaTime)
	: m_deviceContextCached(deviceContext), m_deltaTime(deltaTime)
{
}

void ComponentEntityUpdater::Visit(StaticMeshComponent* staticModelComponent)
{
	staticModelComponent->UpdateAbsoluteEntities();
	UpdateTransformationBuffer(staticModelComponent);
}

void ComponentEntityUpdater::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	skeletalModelComponent->UpdateAbsoluteEntities();
	UpdateTransformationBuffer(skeletalModelComponent);

	AnimationPlayer* animationPlayer = skeletalModelComponent->GetAnimationPlayer();
	if (animationPlayer) animationPlayer->UpdateAnimationPlayer(m_deviceContextCached, m_deltaTime);
}

void ComponentEntityUpdater::Visit(CameraComponent* cameraComponent)
{
	cameraComponent->UpdateAbsoluteEntities();
	cameraComponent->UpdateViewEntity();
	UpdateTransformationBuffer(cameraComponent);

	DynamicBuffer* viewProjMatrixBuffer = cameraComponent->GetViewProjMatrixBuffer();
	viewProjMatrixBuffer->Upload(m_deviceContextCached);
}

void ComponentEntityUpdater::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	sphereCollisionComponent->UpdateAbsoluteEntities();
	UpdateTransformationBuffer(sphereCollisionComponent);
	//UpdateComponentBuffer(sphereCollisionComponent);
	
	sphereCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentEntityUpdater::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	orientedBoxCollisionComponent->UpdateAbsoluteEntities();
	UpdateTransformationBuffer(orientedBoxCollisionComponent);
	//UpdateComponentBuffer(orientedBoxCollisionComponent);

	orientedBoxCollisionComponent->UpdateBoundingVolumeHierarchy();
}

void ComponentEntityUpdater::UpdateComponentBuffer(AComponent* component)
{
	DynamicBuffer* componentBuffer = component->GetComponentBuffer();
	componentBuffer->Upload(m_deviceContextCached);
}

void ComponentEntityUpdater::UpdateTransformationBuffer(AComponent* component)
{
	component->UpdateComponentTransformation();
	DynamicBuffer* transformationBuffer = component->GetTransformationBuffer();
	transformationBuffer->Upload(m_deviceContextCached);
}
