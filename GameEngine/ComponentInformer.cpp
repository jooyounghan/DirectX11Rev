#include "ComponentInformer.h"
#include "StaticModelComponent.h"
#include "SkeletalModelComponent.h"
#include "CameraComponent.h"

using namespace ImGui;

ComponentInformer::ComponentInformer(AssetManager* assetManager, ComponentManager* componentManager)
	: m_assetManagerCached(assetManager), m_componentManagerCached(componentManager)
{
}

void ComponentInformer::Visit(StaticModelComponent* staticModelComponent)
{
	Text("Skeletal Model Component");
	RenderComponent(staticModelComponent);
	RenderModelComponent(staticModelComponent);
	Button(staticModelComponent->GetComponentName().c_str());
}

void ComponentInformer::Visit(SkeletalModelComponent* skeletalModelComponent)
{
	Text("Static Model Component");
	RenderComponent(skeletalModelComponent);
	RenderModelComponent(skeletalModelComponent);
	Button(skeletalModelComponent->GetComponentName().c_str());
}

void ComponentInformer::Visit(CameraComponent* cameraComponent)
{
	Text("Camera Model Component");
	RenderComponent(cameraComponent);

	Button(cameraComponent->GetComponentName().c_str());
}

void ComponentInformer::RenderComponent(AComponent* component)
{
	Text("Transformation");
	AComponent* parentComponent = component->GetParentComponent();
	if (parentComponent != nullptr)
	{
		if(DragFloat3())
	}
	else
	{

	}
}

void ComponentInformer::RenderModelComponent(AModelComponent* modelComponent)
{
	Text("Materials");
}
