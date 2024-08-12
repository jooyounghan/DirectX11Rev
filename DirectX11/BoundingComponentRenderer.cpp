#include "BoundingComponentRenderer.h"
#include "ABoundingComponent.h"
#include "Debugable.h"

#include "PSOObject.h"

#include "Camera.h"
#include "Map.h"


BoundingComponentRenderer::BoundingComponentRenderer(PSOObject* PSOObjectIn)
	: ARenderer(PSOObjectIn)
{
}

void BoundingComponentRenderer::PresetRendering(Camera* CameraIn, Map* MapIn)
{
	ID3D11RenderTargetView* RTVs[] = { CameraIn->GetSceneRTV() };
	D3D11_VIEWPORT Viewports[] = { CameraIn->GetViewport() };
	ID3D11DepthStencilView* DSV = CameraIn->GetSceneDSV();

	PSOObjectCached->SetPipelineObject(1, RTVs, Viewports, DSV);

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	PSOObjectCached->SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void BoundingComponentRenderer::ResetRendering()
{
	PSOObjectCached->ResetVSConstantBuffers(0, 1);
}

void BoundingComponentRenderer::Render(MeshObject* MeshObjectIn)
{
}

void BoundingComponentRenderer::Render(ABoundingComponent* MeshObjectIn)
{
	Debugable* DebugObject = MeshObjectIn->DebugObject;
	ID3D11DeviceContext* DeviceContextCached = MeshObjectIn->DeviceContextCached;

	if (DebugObject)
	{
		ID3D11Buffer* VertexBuffers[] = { DebugObject->GetVertexBuffer() };
		UINT Strides[] = { DebugObject->GetVertexTypeSize() };
		UINT Offsets[] = { 0 };

		DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
		DeviceContextCached->IASetIndexBuffer(DebugObject->GetIndexBuffer(), DebugObject->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { MeshObjectIn->TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { DebugObject->GetDebuggingColorBuffer().GetBuffer() };

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

