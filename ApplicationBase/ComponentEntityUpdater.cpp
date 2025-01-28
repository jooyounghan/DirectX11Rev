#include "ComponentEntityUpdater.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "ASphereCollisionComponent.h"
#include "AOrientedBoxCollisionComponent.h"
#include "AFrustumCollisionComponent.h"

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
	cameraComponent->UpdateViewElement();
	UpdateTransformationBuffer(cameraComponent);

	DynamicBuffer* viewProjMatrixBuffer = cameraComponent->GetViewProjMatrixBuffer();
	viewProjMatrixBuffer->Upload(m_deviceContextCached);
}

void ComponentEntityUpdater::Visit(ARenderSphereCollisionComponent* renderSphereCollisionComponent)
{
	renderSphereCollisionComponent->UpdateAbsoluteEntities();
	UpdateTransformationBuffer(renderSphereCollisionComponent);
	renderSphereCollisionComponent->UpdateBoundginVolumeHierachy();
}

void ComponentEntityUpdater::Visit(ARenderOrientedBoxCollisionComponent* renderOrientedBoxCollisionComponent)
{
	renderOrientedBoxCollisionComponent->UpdateAbsoluteEntities();
	UpdateTransformationBuffer(renderOrientedBoxCollisionComponent);
	renderOrientedBoxCollisionComponent->UpdateBoundginVolumeHierachy();
}

void ComponentEntityUpdater::Visit(ARenderFrustumCollisionComponent* renderFrustumCollisionComponent)
{
	renderFrustumCollisionComponent->UpdateAbsoluteEntities();
	UpdateTransformationBuffer(renderFrustumCollisionComponent);
	renderFrustumCollisionComponent->UpdateBoundginVolumeHierachy();
}

void ComponentEntityUpdater::UpdateTransformationBuffer(AComponent* component)
{
	component->UpdateComponentTransformation();
	DynamicBuffer* transformationBuffer = component->GetTransformationBuffer();
	transformationBuffer->Upload(m_deviceContextCached);
}
