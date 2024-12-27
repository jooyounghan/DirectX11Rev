#pragma once
#include "imgui.h"
#include "IComponentVisitor.h"
#include "ComponentManager.h"

class ComponentEntity;
class AModelComponent;

class ComponentInformer : public IComponentVisitor
{
public:
	ComponentInformer(AssetManager* assetManager, ComponentManager* componentManager);

protected:
	AssetManager* m_assetManagerCached = nullptr;
	ComponentManager* m_componentManagerCached = nullptr;

public:
	virtual void Visit(StaticModelComponent* staticModelComponent) override;
	virtual void Visit(SkeletalModelComponent* skeletalModelComponent) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

private:
	void RenderComponent(AComponent* component);
	void RenderModelComponent(AModelComponent* modelComponent);
};

