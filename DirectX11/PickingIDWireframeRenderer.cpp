#include "PickingIDWireframeRenderer.h"

#include "ABoundingObject.h"
#include "Debugable.h"

#include "PSOObject.h"

PickingIDWireframeRenderer::PickingIDWireframeRenderer(PSOObject* PSOObjectIn)
	: APickingIDRenderer(PSOObjectIn)
{
}

void PickingIDWireframeRenderer::Render(MeshObject* MeshObjectIn)
{
}

void PickingIDWireframeRenderer::Render(ABoundingObject* BoundingObjectIn)
{
	Debugable* DebugObject = BoundingObjectIn->GetDebugObject();

	if (DebugObject)
	{
		ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer() };
		UINT Strides[] = { DebugObject->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };

		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { BoundingObjectIn->TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { BoundingObjectIn->GetPickingIDBufferCached() };

		PSOObjectCached->SetVSConstantBuffers(1, 1, VSConstBuffers);
		PSOObjectCached->SetPSConstantBuffers(0, 1, PSConstBuffers);
#ifdef _DEBUG
		PSOObjectCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

		PSOObjectCached->ResetVSConstantBuffers(1, 1);
		PSOObjectCached->ResetPSConstantBuffers(0, 1);
	}
}