#include "ComponentCollector.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"
#include "SpotLightComponent.h"
#include "PointLightComponent.h"

void ComponentCollector::Visit(StaticMeshComponent* staticModelComponent)
{
	m_collectedComponents.emplace_back(staticModelComponent);
}

void ComponentCollector::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	m_collectedComponents.emplace_back(skeletalModelComponent);
}

void ComponentCollector::Visit(CameraComponent* cameraComponent)
{
	m_collectedComponents.emplace_back(cameraComponent);
}

void ComponentCollector::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	m_collectedComponents.emplace_back(sphereCollisionComponent);
}

void ComponentCollector::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	m_collectedComponents.emplace_back(orientedBoxCollisionComponent);
}

void ComponentCollector::Visit(SpotLightComponent* spotLightComponent)
{
	m_collectedComponents.emplace_back(spotLightComponent);
}

void ComponentCollector::Visit(PointLightComponent* pointLightComponent)
{
	m_collectedComponents.emplace_back(pointLightComponent);
}
