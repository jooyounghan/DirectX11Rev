#pragma once
#include "imgui.h"
#include "IComponentVisitor.h"
#include "ComponentManager.h"

class ComponentInformer : public IComponentVisitor
{
public:
	ComponentInformer(ComponentManager* componentManager);

protected:
	ComponentManager* m_componentManagerCached = nullptr;

public:
	virtual void Visit(StaticModelComponent* staticModelComponent) override;
	virtual void Visit(SkeletalModelComponent* skeletalModelComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;
};

