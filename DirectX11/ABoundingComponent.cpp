#include "ABoundingComponent.h"
#include "GraphicsPipeline.h"
#include "Debugable.h"
#include "PSOObject.h"
#include "ARenderer.h"

ABoundingComponent::ABoundingComponent(GraphicsPipeline* GraphicsPipelineInstances, AssetManager* AssetManagerInstance)
	: AttachableObject(
		GraphicsPipelineInstances->GetDevice(),
		GraphicsPipelineInstances->GetDeviceContext()
	)
{
}

ABoundingComponent::~ABoundingComponent()
{
}

void ABoundingComponent::SetCollisionColor()
{
	if (DebugObject != nullptr)
	{
		if (IsCollided)
		{
			DebugObject->UpdateColor(XMVectorSet(0.f, 1.f, 0.f, 1.f), DeviceContextCached);
		}
		else
		{
			DebugObject->UpdateColor(XMVectorSet(1.f, 0.f, 0.f, 1.f), DeviceContextCached);
		}
	}
}

void ABoundingComponent::UpdateObject(const float& DeltaTimeIn)
{
	AttachableObject::UpdateObject(DeltaTimeIn);
	SetCollisionColor();
}

void ABoundingComponent::AcceptRenderer(ARenderer* Renderer)
{
	AttachableObject::AcceptRenderer(Renderer);
	Renderer->Render(DeviceContextCached, this);
}
