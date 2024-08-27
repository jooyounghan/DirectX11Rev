#include "BoundingComponentRenderer.h"
#include "ABoundingObject.h"
#include "Debugable.h"

#include "PSOObject.h"

#include "Camera.h"
#include "MapAsset.h"


BoundingComponentRenderer::BoundingComponentRenderer(PSOObject* PSOObjectIn)
	: ARenderer(PSOObjectIn)
{
}

void BoundingComponentRenderer::PresetRendering(const unsigned int& RTVCountIn, ID3D11RenderTargetView** RTVsIn, D3D11_VIEWPORT* ViewportIn, ID3D11DepthStencilView* DSVIn, Camera* CameraIn, MapAsset* MapIn)
{
	PSOObjectCached->SetPipelineObject(RTVCountIn, RTVsIn, ViewportIn, DSVIn);

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	PSOObjectCached->SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void BoundingComponentRenderer::ResetRendering()
{
	PSOObjectCached->ResetVSConstantBuffers(0, 1);
}

void BoundingComponentRenderer::Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn)
{
	// Do Nothing
}

void BoundingComponentRenderer::Render(ID3D11DeviceContext* DeviceContextIn, ABoundingObject* MeshObjectIn)
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
		ID3D11Buffer* PSConstBuffers[] = { MeshObjectIn->GetDebuggingColorBuffer().GetBuffer() };

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