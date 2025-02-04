#pragma once
#include "IComponentVisitor.h"

#include <vector>

class ComponentCollector : public IComponentVisitor
{
public:
	ComponentCollector() = default;
	~ComponentCollector() override = default;

protected:
	std::vector<AComponent*> m_collectedComponents;

public:
	inline const std::vector<AComponent*>& GetCollectedComponents() { return m_collectedComponents; }

public:
	virtual void Visit(StaticMeshComponent* staticModelComponent) override;
	virtual void Visit(SkeletalMeshComponent* skeletalModelComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void Visit(SphereCollisionComponent* sphereCollisionComponent) override;
	virtual void Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent) override;

public:
	virtual void Visit(SpotLightComponent* spotLightComponent) override;
	virtual void Visit(PointLightComponent* pointLightComponent) override;
};

