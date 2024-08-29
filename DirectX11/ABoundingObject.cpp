#include "ABoundingObject.h"
#include "GraphicsPipeline.h"
#include "Debugable.h"
#include "PSOObject.h"
#include "ARenderer.h"

ABoundingObject::ABoundingObject(GraphicsPipeline* GraphicsPipelineInstance)
	: AAttachableObject(GraphicsPipelineInstance), DebuggingColorBuffer(GraphicsPipelineInstance->GetDevice())
{
}

ABoundingObject::~ABoundingObject()
{
}

void ABoundingObject::UpdateColor(const XMVECTOR& ColorIn, ID3D11DeviceContext* DeviceContextIn)
{
}

void ABoundingObject::SetCollisionColor()
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

void ABoundingObject::AcceptRenderer(ARenderer* Renderer)
{
	AAttachableObject::AcceptRenderer(Renderer);
	Renderer->Render(DeviceContextCached, this);
}
