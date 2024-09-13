#include "ABoundingObject.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "Debugable.h"
#include "PSOObject.h"
#include "ARenderer.h"

ABoundingObject::ABoundingObject()
	: AAttachableObject(), DebuggingColorBuffer()
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
			DebuggingColorBuffer.Upload(XMVectorSet(0.f, 1.f, 0.f, 1.f));
		}
		else
		{
			DebuggingColorBuffer.Upload(XMVectorSet(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void ABoundingObject::AcceptRenderer(ARenderer* Renderer)
{
	Renderer->Render(this);
	AcceptChildrenRenderer(Renderer);
}
