#include "ABoundingComponent.h"
#include "GraphicsPipeline.h"
#include "Debugable.h"
#include "PSOObject.h"
#include "ARenderer.h"

ABoundingComponent::ABoundingComponent(GraphicsPipeline* GraphicsPipelineInstance)
	: AttachableObject(GraphicsPipelineInstance), DebuggingColorBuffer(GraphicsPipelineInstance->GetDevice())
{
}

ABoundingComponent::~ABoundingComponent()
{
}

void ABoundingComponent::UpdateColor(const XMVECTOR& ColorIn, ID3D11DeviceContext* DeviceContextIn)
{
}

void ABoundingComponent::SetCollisionColor()
{
	if (DebugObject != nullptr)
	{
		if (IsCollided)
		{
			DebuggingColorBuffer.Upload(DeviceContextCached, XMVectorSet(0.f, 1.f, 0.f, 1.f));
		}
		else
		{
			DebuggingColorBuffer.Upload(DeviceContextCached, XMVectorSet(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void ABoundingComponent::AcceptRenderer(ARenderer* Renderer)
{
	AttachableObject::AcceptRenderer(Renderer);
	Renderer->Render(DeviceContextCached, this);
}
