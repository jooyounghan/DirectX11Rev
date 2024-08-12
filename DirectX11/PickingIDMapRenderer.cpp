#include "PickingIDMapRenderer.h"

#include "ABoundingComponent.h"
#include "MeshObject.h"

#include "AMeshAsset.h"
#include "Debugable.h"

#include "PSOObject.h"

#include "Camera.h"
#include "Map.h"

using namespace std;

PickingIDRenderer::PickingIDRenderer(PSOObject* PSOObjectIn)
	: ARenderer(PSOObjectIn)
{
}

void PickingIDRenderer::PresetRendering(
	const unsigned int& RTVCountIn, 
	ID3D11RenderTargetView** RTVsIn, 
	D3D11_VIEWPORT* ViewportIn, 
	ID3D11DepthStencilView* DSVIn, 
	Camera* CameraIn, 
	Map* MapIn
)
{
	PSOObjectCached->SetPipelineObject(RTVCountIn, RTVsIn, ViewportIn, DSVIn);

	ID3D11Buffer* ViewProjBuffer[] = { CameraIn->ViewProjBuffer.GetBuffer() };
	PSOObjectCached->SetVSConstantBuffers(0, 1, ViewProjBuffer);
}

void PickingIDRenderer::ResetRendering()
{
	PSOObjectCached->ResetVSConstantBuffers(0, 1);
}

void PickingIDRenderer::Render(ID3D11DeviceContext* DeviceContextIn, MeshObject* MeshObjectIn)
{
	AMeshAsset* MeshAssetInstance = MeshObjectIn->GetMeshAssetInstance();

	if (MeshAssetInstance)
	{
		const vector<ID3D11Buffer*> VertexBuffers = MeshAssetInstance->GetPositionBuffer();
		const vector<UINT> Strides = MeshAssetInstance->GetPositionStride();
		const vector<UINT> Offsets = MeshAssetInstance->GetPositionOffset();

		DeviceContextIn->IASetVertexBuffers(0, VertexBuffers.size(),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextIn->IASetIndexBuffer(MeshAssetInstance->GetIndexBuffer(), MeshAssetInstance->GetIndexFormat(), 0);

		ID3D11Buffer* VSConstBuffers[] = { MeshObjectIn->TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { MeshObjectIn->GetPickingIDBuffer().GetBuffer() };

		PSOObjectCached->SetVSConstantBuffers(1, 1, VSConstBuffers);
		PSOObjectCached->SetPSConstantBuffers(0, 1, PSConstBuffers);
#ifdef _DEBUG
		PSOObjectCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextIn->DrawIndexed(static_cast<UINT>(MeshAssetInstance->GetIndexCount()), 0, 0);

		PSOObjectCached->ResetVSConstantBuffers(1, 1);
	}
}

void PickingIDRenderer::Render(ID3D11DeviceContext* DeviceContextIn, ABoundingComponent* MeshObjectIn)
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
