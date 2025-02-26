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
}

void ComponentUpdater::Visit(StaticMeshComponent* staticModelComponent)
{
	uint8_t modifiedOption = staticModelComponent->ComsumeModifiedOption();
	UpdateBaseComponent(staticModelComponent, modifiedOption);
}

void ComponentUpdater::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	uint8_t modifiedOption = skeletalModelComponent->ComsumeModifiedOption();
	UpdateBaseComponent(skeletalModelComponent, modifiedOption);

	if (modifiedOption & GetComponentUpdateOption(ESkeletalMeshComponentUpdateOption::ANIMATION_ENTITY))
	{
		AnimationPlayer* animationPlayer = skeletalModelComponent->GetAnimationPlayer();
		if (animationPlayer) animationPlayer->UpdateAnimationPlayer(m_deviceContextCached, m_deltaTime);
	}
}

void ComponentUpdater::Visit(CameraComponent* cameraComponent)
{
	uint8_t modifiedOption = cameraComponent->ComsumeModifiedOption();
	UpdateBaseComponent(cameraComponent, modifiedOption);

	if (modifiedOption & GetComponentUpdateOption(ECameraComponentUpdateOption::VIEW_ENTITY))
	{
		DynamicBuffer& viewEntityBufer = cameraComponent->GetViewEntityBuffer();
		viewEntityBufer.Upload(m_deviceContextCached);
	}
}

void ComponentUpdater::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	uint8_t modifiedOption = sphereCollisionComponent->ComsumeModifiedOption();
	UpdateBaseComponent(sphereCollisionComponent, modifiedOption);

	if (modifiedOption & GetComponentUpdateOption(EComponentUpdateOption::TRANSFORMATION_ENTITY))
	{
		sphereCollisionComponent->UpdateBoundingVolumeHierarchy();
	}
}

void ComponentUpdater::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	uint8_t modifiedOption = orientedBoxCollisionComponent->ComsumeModifiedOption();
	UpdateBaseComponent(orientedBoxCollisionComponent, modifiedOption);

	if (modifiedOption & GetComponentUpdateOption(EComponentUpdateOption::TRANSFORMATION_ENTITY))
	{
		orientedBoxCollisionComponent->UpdateBoundingVolumeHierarchy();
	}
}

void ComponentUpdater::Visit(SpotLightComponent* spotLightComponent)
{
	uint8_t modifiedOption = spotLightComponent->ComsumeModifiedOption();
	UpdateBaseComponent(spotLightComponent, modifiedOption);
	UpdateLightComponent(spotLightComponent, modifiedOption);

	if (modifiedOption & GetComponentUpdateOption(ESpotLightComponentUpdateOption::VIEW_ENTITY))
	{
		StructuredBuffer* viewEntityBuferAddress = spotLightComponent->GetViewEntitiesBufferAddress();
		viewEntityBuferAddress->Upload(m_deviceContextCached);
	}
}

void ComponentUpdater::Visit(PointLightComponent* pointLightComponent)
{
	uint8_t modifiedOption = pointLightComponent->ComsumeModifiedOption();
	UpdateBaseComponent(pointLightComponent, modifiedOption);
	UpdateLightComponent(pointLightComponent, modifiedOption);

	if (modifiedOption & GetComponentUpdateOption(ESpotLightComponentUpdateOption::VIEW_ENTITY))
	{
		for (size_t idx = 0; idx < 6; ++idx)
		{
			StructuredBuffer* viewEntityBufferAddress = pointLightComponent->GetPointLightFrustum(idx).GetViewEntityBufferAddress();
			viewEntityBufferAddress->Upload(m_deviceContextCached);
		}
	}
}

void ComponentUpdater::UpdateBaseComponent(AComponent* component, const UINT8& modifiedOption)
{
	if (modifiedOption & GetComponentUpdateOption(EComponentUpdateOption::COMPONENT_ENTITY))
	{
		DynamicBuffer& componentEntityBuffer = component->GetComponentEntityBuffer();
		componentEntityBuffer.Upload(m_deviceContextCached);
	}
	if (modifiedOption & GetComponentUpdateOption(EComponentUpdateOption::TRANSFORMATION_ENTITY))
	{
		DynamicBuffer& transformationEntityBuffer = component->GetTransformationEntityBuffer();
		transformationEntityBuffer.Upload(m_deviceContextCached);
	}
}

void ComponentUpdater::UpdateLightComponent(LightComponent* lightComponent, const UINT8& modifiedOption)
{
	if (modifiedOption & GetComponentUpdateOption(ELightComponentUpdateOption::LIGHT_ENTITY))
	{
		StructuredBuffer* lightEntityBufferAddress = lightComponent->GetLightEntityBufferAddress();
		lightEntityBufferAddress->Upload(m_deviceContextCached);
	}
}
