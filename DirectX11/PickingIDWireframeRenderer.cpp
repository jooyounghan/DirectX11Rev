#include "PickingIDWireframeRenderer.h"

#include "ABoundingComponent.h"
#include "Debugable.h"

#include "PSOObject.h"

PickingIDWireframeRenderer::PickingIDWireframeRenderer(PSOObject* PSOObjectIn)
	: APickingIDRenderer(PSOObjectIn)
{
}

void PickingIDWireframeRenderer::Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn)
{
}

void PickingIDWireframeRenderer::Render(ID3D11DeviceContext* DeviceContextIn, ABoundingComponent* MeshObjectIn)
{
	Debugable* DebugObject = MeshObjectIn->GetDebugObject();

	if (DebugObject)
	{
		ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer() };
		UINT Strides[] = { DebugObject->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };

		DeviceContextIn->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextIn->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { MeshObjectIn->TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { MeshObjectIn->GetPickingIDBuffer().GetBuffer() };

		PSOObjectCached->SetVSConstantBuffers(1, 1, VSConstBuffers);
		PSOObjectCached->SetPSConstantBuffers(0, 1, PSConstBuffers);
#ifdef _DEBUG
		PSOObjectCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextIn->DrawIndexed(static_cast<UINT>(DebugObject->GetIndexCount()), 0, 0);

		PSOObjectCached->ResetVSConstantBuffers(1, 1);
		PSOObjectCached->ResetPSConstantBuffers(0, 1);
	}
}
