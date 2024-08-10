#include "ABoundingComponent.h"
#include "GraphicsPipeline.h"
#include "Debugable.h"
#include "PSOObject.h"

ABoundingComponent::ABoundingComponent(GraphicsPipeline* GraphicsPipelineInstances, AssetManager* AssetManagerInstance)
	: RelativePlaceableObject(
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
	RelativePlaceableObject::UpdateObject(DeltaTimeIn);
	SetCollisionColor();
}

void ABoundingComponent::Render(PSOObject* PSOObjectIn)
{
	if (DebugObject)
	{
		ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer() };
		UINT Strides[] = { DebugObject->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };
		
		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { GetPickingIDBuffer().GetBuffer(), DebugObject->GetDebuggingColorBuffer().GetBuffer()};

		PSOObjectIn->SetVSConstantBuffers(1, 1, VSConstBuffers);
		PSOObjectIn->SetPSConstantBuffers(0, 2, PSConstBuffers);
#ifdef _DEBUG
		PSOObjectIn->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

		PSOObjectIn->ResetVSConstantBuffers(1, 1);
		PSOObjectIn->ResetPSConstantBuffers(0, 2);
	}
}
