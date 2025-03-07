#include "ComponentUpdater.h"

#include "Scene.h"
#include "LightManager.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "DynamicBuffer.h"
#include "StructuredBuffer.h"

#include <functional>

using namespace std;

ComponentUpdater::ComponentUpdater(ID3D11DeviceContext* deviceContext, const float& deltaTime)
	: m_deviceContextCached(deviceContext), m_deltaTime(deltaTime)
{
}

void ComponentUpdater::Visit(Scene* scene)
{
	LightManager& lightManager = scene->GetLightManager();
	if (lightManager.ConsumeIsLightCountChanged())
	{
		DynamicBuffer& lightManagerEntityBuffer = lightManager.GetLightManagerEntityBuffer();
		lightManagerEntityBuffer.Upload(m_deviceContextCached);
	}

	vector<reference_wrapper<StructuredBuffer>> structureBuffers{
		lightManager.GetSpotLightEntitiesBuffer(),
		lightManager.GetSpotLightViewEntitiesBuffer(),
		lightManager.GetPointLightEntitiesBuffer(),
		lightManager.GetPointLightPositionsBuffer()
	};
	for (auto& structureBuffer : structureBuffers)
	{
		StructuredBuffer& sb = structureBuffer.get();
		if (sb.GetBufferChangedFlag().ConsumeFlag())
		{
			sb.Upload(m_deviceContextCached);
		}
	}
}

void ComponentUpdater::Visit(StaticMeshComponent* staticModelComponent)
{
	UpdateBaseComponent(staticModelComponent);
}

void ComponentUpdater::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	UpdateBaseComponent(skeletalModelComponent);

	AnimationPlayer* animationPlayer = skeletalModelComponent->GetAnimationPlayer();
	if (animationPlayer->IsPlaying()) animationPlayer->UpdateAnimationPlayer(m_deviceContextCached, m_deltaTime);
}

void ComponentUpdater::Visit(CameraComponent* cameraComponent)
{
	UpdateBaseComponent(cameraComponent);

	AtomicFlag& viewUpdatedFlag = cameraComponent->GetViewUpdatedFlag();
	if (viewUpdatedFlag.ConsumeFlag())
	{
		cameraComponent->UpdateViewEntity(m_deviceContextCached);
	}

	AtomicFlag& cameraUpdatedFlag = cameraComponent->GetCameraUpdatedFlag();
	if (cameraUpdatedFlag.ConsumeFlag())
	{
		cameraComponent->GetCameraEntityBuffer().Upload(m_deviceContextCached);
	}
}

void ComponentUpdater::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	UpdateBaseComponent(sphereCollisionComponent);
}

void ComponentUpdater::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	UpdateBaseComponent(orientedBoxCollisionComponent);
}

void ComponentUpdater::Visit(SpotLightComponent* spotLightComponent)
{
	UpdateBaseComponent(spotLightComponent);

	AtomicFlag& viewUpdatedFlag = spotLightComponent->GetViewUpdatedFlag();
	if (viewUpdatedFlag.ConsumeFlag())
	{
		spotLightComponent->UpdateViewEntity(m_deviceContextCached);
	}
}

void ComponentUpdater::Visit(PointLightComponent* pointLightComponent)
{
	UpdateBaseComponent(pointLightComponent);

	AtomicFlag& viewUpdatedFlag = pointLightComponent->GetViewUpdatedFlag();
	if (viewUpdatedFlag.ConsumeFlag())
	{
		pointLightComponent->UpdatePointLightFrustums(m_deviceContextCached);
	}

}

void ComponentUpdater::UpdateBaseComponent(AComponent* component)
{
	AtomicFlag& transformationEntityUpdatedFlag = component->GetTransformationEntityUpdatedFlag();
	if (transformationEntityUpdatedFlag.ConsumeFlag())
	{
		component->UpdateEntity(m_deviceContextCached);
	}
}