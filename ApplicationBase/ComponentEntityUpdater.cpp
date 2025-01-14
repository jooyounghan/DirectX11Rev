#include "ComponentEntityUpdater.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

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

void ComponentEntityUpdater::UpdateTransformationBuffer(ComponentEntity* componentEntity)
{
	componentEntity->UpdateComponentTransformation();
	DynamicBuffer* transformationBuffer = componentEntity->GetTransformationBuffer();
	transformationBuffer->Upload(m_deviceContextCached);
}
