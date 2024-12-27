#include "ComponentInformer.h"
#include "StaticModelComponent.h"
#include "SkeletalModelComponent.h"
#include "CameraComponent.h"

using namespace ImGui;

ComponentInformer::ComponentInformer(ComponentManager* componentManager)
	: m_componentManagerCached(componentManager)
{
}

void ComponentInformer::Visit(StaticModelComponent* staticModelComponent)
{
	Text("Skeletal Model Component");
	Button(staticModelComponent->GetComponentName().c_str());
}

void ComponentInformer::Visit(SkeletalModelComponent* skeletalModelComponent)
{
	Text("Static Model Component");
	Button(skeletalModelComponent->GetComponentName().c_str());
}

void ComponentInformer::Visit(CameraComponent* cameraComponent)
{
	Text("Camera Model Component");
	Button(cameraComponent->GetComponentName().c_str());
}
